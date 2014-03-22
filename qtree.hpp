#ifndef QTREE_HPP
#define QTREE_HPP

#include "grafikus_rutinok.hpp"
//#include <vector>
#include <set>
#include <utility>

#ifdef __TESZTELUNK__
  #include "SFML/Graphics.hpp"
  #include <iostream>
  #include <stdexcept>
  std::ostream& operator<< (std::ostream&, const Box&);
#endif

/*
  T egy regularis tipus, van egy Box tipusu box()-a
*/
template <class T>
class QTree {
public:
  //Maximalis elemek szama egy teruleten belul
  static constexpr int kapacitas = 4;
  //Minimalis terulet-szelesseg/magassag
  static constexpr int minDim    = 2;

  QTree (Box bound);
  ~QTree();
  //Egyenlore legalabbis nem kell a masolas
  QTree (const QTree&) = delete;
  QTree& operator= (const QTree&) = delete;

  void insert(T* elem)
    {root->insert(elem,elem->box());}
  void erase (T* elem)
    {root->erase(elem); }
  void move (T* elem, const Box& ujhely)
    {root->move(elem,ujhely); }
  std::set<T*> find (Vektor pont) const
    {return root->find(pont);}
  std::pair<std::set<T*>, Box> findLeaf (Vektor pont) const
    {return root->findLeaf(pont);}
  std::set<T*> find (const Box& terulet) const
    {return root->find(terulet);}
  void clear();

  //A teszteleshez
#ifdef __TESZTELUNK__
    void draw(sf::RenderWindow& w) const;
    void printLeafSize (Vektor p) const {root->printLeafSize(p);}
#endif
private:
  class Node {
    size_t size {0};
    Box boundary;
    std::set<T*> elements {};
    Node *upleft {}, *upright {},
         *downleft {}, *downright {};

    void _subdivide ();
    void _merge ();
  public:
    explicit Node (Box bound) : boundary{bound} {  }
    ~Node ();
    bool insert(T*,const Box&);
    bool erase (T*);
    void move (T*,const Box&);
    const Box& bound() const
      {return boundary;}
    std::set<T*> find(Vektor) const;
    std::pair<std::set<T*>, Box> findLeaf (Vektor pont) const;
    std::set<T*> find(const Box&) const;
    bool isLeaf () const
      { return (upleft == nullptr); }
    //Csak leafeknel van ertelme
    const std::set<T*>& getElements () const
      { return elements; }
#ifdef __TESZTELUNK__
    void draw(sf::RenderWindow& w) const;
    void printLeafSize(Vektor p) const {
      if (!boundary.in(p)) {return;}
      if (isLeaf()) {
        std::cerr<<"  "<<size<<" "<<elements.size()<<std::endl;
      } else {
        upleft->printLeafSize(p);
        upright->printLeafSize(p);
        downleft->printLeafSize(p);
        downright->printLeafSize(p);
      }
    }
    void testSize() const {
      std::set<T*> s = find(boundary);
      for (auto e:s) {
        if (!intersect(boundary,e->box())) {throw std::runtime_error("valaki nincs benne");}
      }
      if (size != s.size()) {
        std::cerr<<"[ ERROR ] Size nem egyezik:\n";
        std::cerr<<size<<" "<<s.size()<<std::endl;
        std::cerr<<boundary<<std::endl;
        throw std::runtime_error("size nem egyezik");
      }
    }
#endif
  }; //end node

  Node* root;
}; //end qtree

template<class T>
QTree<T>::Node::~Node () {
  if (!isLeaf()) {
    delete upleft;
    delete upright;
    delete downleft;
    delete downright;
  }
}

template<class T>
void QTree<T>::Node::_subdivide () {
  Vektor middle = {(boundary.bal()+boundary.jobb())/2,
                   (boundary.felso()+boundary.also())/2};
  upleft = new Node(Box(boundary.bf(),middle));
  upright = new Node(Box({middle.x,boundary.felso()},
                         {boundary.jobb(),middle.y}));
  downleft = new Node(Box({boundary.bal(),middle.y},
                         {middle.x,boundary.also()}));
  downright = new Node(Box(middle,boundary.ja()));
}

template<class T>
void QTree<T>::Node::_merge () {
  std::set<T*> also = upleft->getElements();
  elements.insert(also.begin(), also.end());
  also = upright->getElements();
  elements.insert(also.begin(), also.end());
  also = downleft->getElements();
  elements.insert(also.begin(), also.end());
  also = downright->getElements();
  elements.insert(also.begin(), also.end());
  delete upleft;   delete upright;
  delete downleft; delete downright;
  upleft = nullptr;
}

template<class T>
bool QTree<T>::Node::insert (T* elem, const Box& b) {
  if (!intersect(b,boundary))
    {return false;}
  ++size;
  if (!isLeaf()) {
    upleft->insert(elem,b);
    upright->insert(elem,b);
    downleft->insert(elem,b);
    downright->insert(elem,b);
    return true;
  }
  if (size>QTree::kapacitas
      && boundary.szeles()/2>=QTree::minDim
      && boundary.magas()/2>=QTree::minDim) {
    _subdivide();
    size=0;
    for (auto i = elements.begin(); i!=elements.end(); ++i) {
      if (!insert(*i,(*i)->box())) {throw std::runtime_error("nem rakta be");}
    }
    elements.clear();
    insert(elem,b);
  } else {
    elements.insert(elem);
  }
  return true;
}

template<class T>
bool QTree<T>::Node::erase (T* elem) {
  if (!intersect(elem->box(),boundary))
    { return false; }
  if (isLeaf()) {
    auto i = elements.find(elem);
    if (i!=elements.end()) {
      elements.erase(i);
      --size;
      return true;
    } else {
      return false;
    }
  }
  bool toroltunk = upleft->erase(elem);
  toroltunk |= upright->erase(elem);
  toroltunk |= downleft->erase(elem);
  toroltunk |= downright->erase(elem);
  if (toroltunk) { --size; }
  if (size < kapacitas) {       //direkt kevesebb egyel, igy nem kell rogton mergelni
    //elements.reserve(size);
    _merge();
  }
  return toroltunk;
}

//elem a regi helyen legyen meg
template<class T>
void QTree<T>::Node::move (T* elem, const Box& ujhely) {
  if (!intersect(ujhely,boundary)) {
    erase(elem);
  } else if (intersect(elem->box(),boundary)) {
    if (!isLeaf()) {
      upleft->move(elem,ujhely);
      upright->move(elem,ujhely);
      downleft->move(elem,ujhely);
      downright->move(elem,ujhely);
    }
  } else {
    insert(elem,ujhely);
  }
}

template<class T>
std::set<T*> QTree<T>::Node::find (Vektor pont) const {
  if (!boundary.in(pont))
    {return std::set<T*> {};}
  if (!isLeaf()) {
    std::set<T*> eredm {};
    if ((eredm = upleft->find(pont)).size()) {return eredm;}
    if ((eredm = upright->find(pont)).size()) {return eredm;}
    if ((eredm = downleft->find(pont)).size()) {return eredm;}
    if ((eredm = downright->find(pont)).size()) {return eredm;}
    return eredm;
  }
  return elements;
}

template<class T>
std::pair<std::set<T*>, Box>
     QTree<T>::Node::findLeaf (Vektor pont) const  {
  if (!boundary.in(pont)) {
    return std::make_pair(std::set<T*> {},
             Box(boundary.bf()-Vektor{50,50},boundary.bf()-Vektor{50,50}));
  }
  if (!isLeaf()) {
    std::pair<std::set<T*>, Box> eredm = std::make_pair(std::set<T*> {},
             Box(boundary.bf()-Vektor{50,50},boundary.bf()-Vektor{50,50}));
    if ((eredm = upleft->findLeaf(pont)).second.bal()>=upleft->boundary.bal()-1)
      {return eredm;}
    if ((eredm = upright->findLeaf(pont)).second.bal()>=upright->boundary.bal()-1)
      {return eredm;}
    if ((eredm = downleft->findLeaf(pont)).second.bal()>=downleft->boundary.bal()-1)
      {return eredm;}
    if ((eredm = downright->findLeaf(pont)).second.bal()>=downright->boundary.bal()-1)
      {return eredm;}
    return eredm;
  }
  return std::make_pair(elements,boundary);
}

template<class T>
std::set<T*> QTree<T>::Node::find (const Box& terulet) const {
  if (!intersect(boundary,terulet))
    {return std::set<T*> {};}
  if (!isLeaf()) {
    std::set<T*> eredm {};
    std::set<T*> also = upleft->find(terulet);
    eredm.insert(also.begin(),also.end());
    also = upright->find(terulet);
    eredm.insert(also.begin(),also.end());
    also = downleft->find(terulet);
    eredm.insert(also.begin(),also.end());
    also = downright->find(terulet);
    eredm.insert(also.begin(),also.end());
    return eredm;
  }
  return elements;
}

template<class T>
QTree<T>::QTree (Box bound) {
  root = new Node(bound);
}

template<class T>
QTree<T>::~QTree () {
  delete root;
}

template<class T>
void QTree<T>::clear () {
  Box bound = root->bound();
  delete root;
  root = new Node(bound);
}

#ifdef __TESZTELUNK__

template<class T>
void QTree<T>::Node::draw (sf::RenderWindow& w) const {
  sf::RectangleShape sh (sf::Vector2f(boundary.szeles()-1,boundary.magas()-1));
  sh.setPosition(boundary.bal(),boundary.felso());
  sh.setFillColor(sf::Color::Transparent);
  sh.setOutlineThickness(1);
  sh.setOutlineColor(sf::Color(255,255,255));
  w.draw(sh);
  if (!isLeaf()) {
    upleft->draw(w);
    upright->draw(w);
    downleft->draw(w);
    downright->draw(w);
  }
}

template<class T>
void QTree<T>::draw (sf::RenderWindow& w) const {
  root->draw(w);
}

#endif

#endif  //header guard
