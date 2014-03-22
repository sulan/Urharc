#ifndef TEST_KOCKAK_HPP
#define TEST_KOCKAK_HPP

#include "qtree.hpp"
#include <SFML/Graphics.hpp>
#include <random>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <chrono>

constexpr size_t KockaN = 30;
constexpr double KockaHelykoz = 5;
constexpr int XX = 600;
constexpr int YY = 600;
constexpr int SEB = 50;

class Kocka {      //Ami amugy negyzet
  Box alak;
  Vektor seb;
  QTree<Kocka> *tree;
  sf::Color clr {sf::Color::White};
public:
  Kocka (Box alak_, Vektor seb_, QTree<Kocka> *tree_)
    : alak{alak_}, seb{seb_}, tree{tree_} {
    tree_->insert(this);
  }
  ~Kocka () {
    tree->erase(this);
  }
  sf::Color getColor () const {
    return clr;
  }
  const Box& box () const { return alak; }
  void draw (sf::RenderWindow& w) {
    sf::RectangleShape sh (sf::Vector2f(alak.szeles(),alak.magas()));
    sh.setPosition(alak.bf().x,alak.bf().y);
    sh.setFillColor(clr);
    w.draw(sh);
  }
  void calcIsect () {
    std::set<Kocka*> v = tree->find(alak);
    for (auto e:v) {
      if (e!=this && intersect(alak, e->box())) {
        clr = sf::Color::Red;
        break;
      }
    }
  }
  bool canFind (Kocka* other) {
    std::set<Kocka*> s = tree->find(alak);
    return (s.find(other)!=s.end());
  }
  void move () {
    Box ujhely = alak+seb;
    tree->move(this,ujhely);
    alak=ujhely;
    clr = sf::Color::White;
    if ((ujhely.felso()<0 && seb.y<0) || (ujhely.also()>YY && seb.y>0))
      {seb.y *= -1;}
    if ((ujhely.bal()<0 && seb.x<0) || (ujhely.jobb()>XX && seb.x>0))
      {seb.x *= -1;}
  }
};

void general (std::vector<Kocka*>& v,QTree<Kocka> * t) {
  std::random_device dev; std::default_random_engine e {dev()};
  std::normal_distribution<double> dist (40,10);
  double hely = 1;
  for (size_t i = 0; i< KockaN; ++i) {
    Vektor bf {hely,hely}, ja {0,dist(e)};
    ja.x = ja.y;
    Vektor seb = {(((dist(e) - 30)/10)),(((dist(e) - 30)/10))};
    ja+=bf;
    v[i] = (new Kocka(Box(bf,ja),seb,t));
    hely+=KockaHelykoz;
  }
}

bool ellenoriz (const std::vector<Kocka*>& v) {
  for (size_t i = 0; i+1<v.size(); ++i) {
    for (size_t j = i+1; j<v.size(); ++j) {
      if (intersect(v[i]->box(), v[j]->box()) &&
           !(v[i]->getColor() == sf::Color::Red &&
              v[j]->getColor() == sf::Color::Red)) {
        std::cerr<<"[ ERROR ]: "<<i<<"("<<v[i]<<")"<<" & "<<j<<"("<<v[j]<<")"<<"utkozott, es nem vettek eszre:\n";
        std::cerr<<v[i]->box().bf().x<<" "<<v[i]->box().bf().y<<" "<<v[i]->box().ja().x<<" "<<v[i]->box().ja().y<<" "<<(v[i]->getColor()==sf::Color::Red)<<std::endl;
        std::cerr<<v[j]->box().bf().x<<" "<<v[j]->box().bf().y<<" "<<v[j]->box().ja().x<<" "<<v[j]->box().ja().y<<" "<<(v[j]->getColor()==sf::Color::Red)<<std::endl;
        std::cerr<<v[i]->canFind(v[j])<<" "<<v[j]->canFind(v[i])<<std::endl;
        return false;
      }
    }
  }
  return true;
}

void testKocka () {
  std::vector<Kocka*> v(KockaN);
  QTree<Kocka> qt (Box({1,1},{XX,YY}));
  general(v,&qt);
  sf::RenderWindow window (sf::VideoMode(XX,YY),"QTreeTest",
                              sf::Style::None);
  sf::Clock clock {};
  auto itt = std::chrono::high_resolution_clock::now();
  auto olditt = std::chrono::high_resolution_clock::now();
  while (window.isOpen()) {
    sf::Event ev;
    while (window.pollEvent(ev)) {
      if (ev.type == sf::Event::Closed
		        || (ev.type == sf::Event::KeyPressed
		        	&& ev.key.code==sf::Keyboard::Escape)) {
        window.close();
      }
    }
    if (clock.getElapsedTime().asMilliseconds()>=SEB) {
      //std::cerr<<clock.getElapsedTime().asMilliseconds()<<std::endl;
      itt = std::chrono::high_resolution_clock::now();
      std::cerr<<std::chrono::duration_cast<std::chrono::milliseconds>(itt-olditt).count()<<std::endl;
      olditt = itt;
      for (auto e:v) {e->move();}
      for (auto e:v) {e->calcIsect();}
      clock.restart();
    //}
    //{
      window.clear();
      qt.draw(window);
      for (auto e:v) {e->draw(window);}
      window.display();
      //assert(ellenoriz(v));
    }
  }
  for (auto e:v) {delete e;}
}

#endif
