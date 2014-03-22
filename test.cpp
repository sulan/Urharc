#include <iostream>
#include "grafikus_rutinok.hpp"
#include "qtree.hpp"
#include <array>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <random>
#include <chrono>
//#include "test_kockak.hpp"
#include "game_engine.hpp"

using namespace std;

ostream& operator<< (ostream& lhs, const Box& rhs) {
  lhs<<"{"<<rhs.bf().x<<", "<<rhs.bf().y<<", "<<rhs.ja().x<<", "<<rhs.ja().y<<"}\n";
  return lhs;
}

struct QTTester {
private:
  Box _box;
  sf::RectangleShape sh;
public:
  QTTester (Box b) : _box{b}, sh(sf::Vector2f(b.szeles(),b.magas())) {
    sh.setFillColor(sf::Color(255,255,255));
    sh.setPosition(_box.bal(),_box.felso());
  }
  Box box() const {return _box;}
  void ccolor () { sh.setFillColor(sf::Color(255,0,0)); }
  void draw (sf::RenderWindow& w) const {
    w.draw(sh);
  }
};

template class QTree<QTTester>;

void testVektor () {
  cout<<"*** TestVektor ***\n";
  Vektor a {1,2}, b{-2,1};
  cout<<"+ "<<(a+b).x<<" "<<(a+b).y<<endl;
  cout<<"- "<<(a-b).x<<" "<<(a-b).y<<endl;
  cout<<"* "<<(a*2).x<<" "<<(a*2).y<<endl;
  cout<<"ph"<<parhuzamos(a,-1*a)<<parhuzamos(a,b)<<endl;
}

void testBox () {
  cout<<"*** TestBox ***\n";
  Box a ({0,0},{1,1}), b ({0,1},{1,2});
  cout<<"dim "<<a.szeles()<<b.magas()<<endl;
  cout<<"benne(101)"<<a.in({0.5,0.5})<<a.in({0,1})<<a.in({0,0})<<endl;
  cout<<"011="<<(a==b)<<(a==a)<<(Box{a}==a)<<endl;
  cout<<"Intersect\n";
  cout<<"01:"<<intersect(a,b)<<intersect(a,a)<<endl;
  Box c ({1,1},{0,0});
  cout<<"0011="<<c.bf().x<<c.bf().y<<c.ja().x<<c.ja().y<<endl;
}

void testEgyenes () {
  cout<<"*** TestEgyenes ***\n";
  Egyenes a ({0,0},0), b ({0,0},{1,0});
  cout<<"iranyv "<<a.iranyV().x<<a.iranyV().y<<" "<<b.iranyV().x<<b.iranyV().y;
  cout<<endl<<"iranyb "<<b.irany()<<endl;
  Egyenes c ({1,1},{2,0}), d ({5,5},{0,1});
  cout<<"ph(0111)"<<parhuzamos(a,b)<<parhuzamos(b,c);
  cout<<parhuzamos(a,d)<<parhuzamos(a,Egyenes{a})<<endl;
  cout<<"SetIrany\n";
  a.setIrany({0,1});  b.setIrany(0);
  cout<<"PI="<<a.irany()<<";0="<<b.irany()<<endl;
}

void testIntersects () {
  constexpr int HaromszogN = 50;
  constexpr int TeglalapN =  50;
  std::minstd_rand d (std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<double> dist (0,500);
  std::vector<std::vector<Vektor>> v {};
  for (size_t i = 0; i<HaromszogN; ++i) {
    Vektor pos {dist(d),dist(d)}, irany {dist(d),dist(d)};
    irany *= 1.0/irany.abs();
    v.push_back(generateHaromszog(pos,irany));
    assert(isClockwise(v[i]));
  }
  for (size_t i = HaromszogN; i<HaromszogN+TeglalapN; ++i) {
    v.push_back(generateTeglalap({dist(d),dist(d)},{dist(d),dist(d)}));
    assert(isClockwise(v[i]));
  }
  //Egyezes
  /*
  for (size_t i = 0; i<HaromszogN+TeglalapN-1; ++i) {
    for (size_t j = i+1; j<HaromszogN+TeglalapN; ++j) {
      bool egy = intersect(v[i],v[j]), ketto = intersect2(v[i],v[j]);
      if (egy!=ketto) {
        std::cerr<<"[ ERROR ] elternek: "<<egy<<" "<<ketto<<std::endl;
        std::cerr<<"  ";
        for (size_t k = 0; k<v[i].size(); ++k) {
          std::cerr<<"  "<<v[i][k].x<<" "<<v[i][k].y<<"\n";
        }
        std::cerr<<std::endl<<"  ";
        for (size_t k = 0; k<v[j].size(); ++k) {
          std::cerr<<"  "<<v[j][k].x<<" "<<v[j][k].y<<"\n";
        }
        std::cerr<<std::endl;
      }
    }
  }  */
  //Verseny  (eredmeny: isec1 nyert 9 mseccel a tervezett 100 elemre)
  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i<HaromszogN+TeglalapN-1; ++i) {
    for (size_t j = i+1; j<HaromszogN+TeglalapN; ++j) {
      intersect(v[i],v[j]);
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout<<"Intersect1: "<<std::chrono::duration_cast<std::chrono::microseconds>
                               (end-start).count()<<std::endl;
  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i<HaromszogN+TeglalapN-1; ++i) {
    for (size_t j = i+1; j<HaromszogN+TeglalapN; ++j) {
      intersect2(v[i],v[j]);
    }
  }
  end = std::chrono::high_resolution_clock::now();
  std::cout<<"Intersect2: "<<std::chrono::duration_cast<std::chrono::microseconds>
                               (end-start).count();
}


void testIsect1 () {
  std::vector<Vektor> v1 = {
    {266.844 ,389.95},
    {261.891 ,380.91},
    {258.22  ,384.304}
  };
  std::vector<Vektor> v2 = {
    {267.874 ,388.954 },
    {263.27  ,379.731 },
    {259.472 ,382.983 }
  };
  std::cout<<intersect(v1,v2)<<std::endl;
  std::cout<<intersect2(v1,v2)<<std::endl;
}


//Korantsem eleg, de keves az ido
void testLaserMetszes () {
  Vektor pos {1,1}, irany{1,0}, a {2,0},b{1,6};
  assert(laserMetszi(pos,irany,a,b)>0);
  a = {0,0};
  assert(laserMetszi(pos,irany,a,b)<0);
  a = {0,1};
  assert(laserMetszi(pos,irany,a,b)<0);
  a = {6,6};
  assert(laserMetszi(pos,irany,a,b)<0);
  a = {2,1};
  assert(laserMetszi(pos,irany,a,b)==1);
  b = {3,1};
  assert(laserMetszi(pos,irany,a,b)==1);
  b = {-1,1};
  assert(laserMetszi(pos,irany,a,b)<0);
  a = {-1.5,1};
  assert(laserMetszi(pos,irany,a,b)<0);
  a = {2,2}; b={3,2};
  assert(laserMetszi(pos,irany,a,b)<0);
  a = {1.5,1}; b={1.75,1};
  assert(laserMetszi(pos,irany,a,b)==0.5);
}


void testQTree() {
  QTree<QTTester> tree (Box({1,1},{500,500}));
  vector<QTTester*> v {};
  sf::ContextSettings anti_aliased;
  anti_aliased.antialiasingLevel = 4;
  sf::RenderWindow window (sf::VideoMode(500,500),"QTreeTest",
                              sf::Style::None,anti_aliased);
  while (window.isOpen()) {
    sf::Event ev;
    while (window.pollEvent(ev)) {
      if (ev.type == sf::Event::Closed
		        || (ev.type == sf::Event::KeyPressed
		        	&& ev.key.code==sf::Keyboard::Escape)) {
        window.close();
      } else
      if (ev.type == sf::Event::MouseButtonPressed) {
        if (ev.mouseButton.button == sf::Mouse::Left) {
          double x = ev.mouseButton.x, y = ev.mouseButton.y;
          v.push_back(new QTTester(Box({x,y},
                                       {x+2,y+2})));
          tree.insert(v.back());
        } else
        if (ev.mouseButton.button == sf::Mouse::Right) {
          double x = ev.mouseButton.x, y = ev.mouseButton.y;
          set<QTTester*> v2 = tree.find({x,y});
          for (auto e:v2) {e->ccolor();}
        }
      }
    }
    window.clear();
    tree.draw(window);
    for (auto e:v) {e->draw(window);}
    window.display();
  }
  for (auto e:v) {delete e;}
}

void testQTree2() {
  QTree<QTTester> tree (Box({0,0},{500,500}));
  array<QTTester,5> pontok {{
              QTTester(Box({10,10},{11,11})),
              QTTester(Box({260,10},{261,11})),
              QTTester(Box({10,260},{11,261})),
              QTTester(Box({260,260},{261,261})),
              QTTester(Box({240,240},{260,260}))
                           }};
  tree.insert(&pontok[0]);
  tree.insert(&pontok[1]);
  tree.insert(&pontok[2]);
  tree.insert(&pontok[3]);
  tree.insert(&pontok[4]);
  assert(tree.find(Box({1,1},{499,499})).size()==5);
  assert(tree.find({1,1}).size() == 2);
  tree.erase(&pontok[4]);
  assert(tree.find({1,1}).size() == 1);
  tree.erase(&pontok[0]);
  assert(tree.find({1,1}).size() == 3);
  tree.erase(&pontok[1]); tree.erase(&pontok[2]);
  tree.erase(&pontok[3]);
}

void testQTreeSize () {
  QTree<QTTester> tree (Box({1,1},{500,500}));
  vector<QTTester*> v {};
  sf::ContextSettings anti_aliased;
  anti_aliased.antialiasingLevel = 4;
  sf::RenderWindow window (sf::VideoMode(500,500),"QTreeTest",
                              sf::Style::None,anti_aliased);
  while (window.isOpen()) {
    sf::Event ev;
    while (window.pollEvent(ev)) {
      if (ev.type == sf::Event::Closed
		        || (ev.type == sf::Event::KeyPressed
		        	&& ev.key.code==sf::Keyboard::Escape)) {
        window.close();
      } else
      if (ev.type == sf::Event::MouseButtonPressed) {
        if (ev.mouseButton.button == sf::Mouse::Left) {
          double x = ev.mouseButton.x, y = ev.mouseButton.y;
          v.push_back(new QTTester(Box({x,y},
                                       {x+10,y+10})));
          tree.insert(v.back());
        } else
        if (ev.mouseButton.button == sf::Mouse::Right) {
          double x = ev.mouseButton.x, y = ev.mouseButton.y;
          tree.printLeafSize({x,y});
        }
      } else if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::A) {
          int a = 0;
          cin>>a;
          auto i = v.begin()+a;
          tree.erase(*i); delete *i;
          v.erase(i);
        }
      }
    }
    window.clear();
    tree.draw(window);
    for (auto e:v) {e->draw(window);}
    window.display();
  }
  for (auto e:v) {delete e;}
}

void testQTreeFindLeaf() {
  QTree<QTTester> tree (Box({1,1},{500,500}));
  vector<QTTester*> v {};
  sf::ContextSettings anti_aliased;
  anti_aliased.antialiasingLevel = 4;
  sf::RenderWindow window (sf::VideoMode(500,500),"QTreeTest",
                              sf::Style::None,anti_aliased);
  while (window.isOpen()) {
    sf::Event ev;
    while (window.pollEvent(ev)) {
      if (ev.type == sf::Event::Closed
		        || (ev.type == sf::Event::KeyPressed
		        	&& ev.key.code==sf::Keyboard::Escape)) {
        window.close();
      } else
      if (ev.type == sf::Event::MouseButtonPressed) {
        if (ev.mouseButton.button == sf::Mouse::Left) {
          double x = ev.mouseButton.x, y = ev.mouseButton.y;
          v.push_back(new QTTester(Box({x,y},
                                       {x+2,y+2})));
          tree.insert(v.back());
        } else
        if (ev.mouseButton.button == sf::Mouse::Right) {
          double x = ev.mouseButton.x, y = ev.mouseButton.y;
          set<QTTester*> v2 = tree.find({x,y});
          auto v3 = tree.findLeaf({x,y});
          //for (auto e:v2) {std::cout<<e<<"  ";}  std::cout<<std::endl;
          //for (auto e:v3.first) {std::cout<<e<<"  ";} std::cout<<std::endl;
          assert(v3.first==v2);
          std::cout<<v3.second.bf().x<<" "<<v3.second.bf().y<<std::endl;
          std::cout<<"\t"<<v3.second.ja().x<<" "<<v3.second.ja().y<<std::endl;
        }
      }
    }
    window.clear();
    tree.draw(window);
    for (auto e:v) {e->draw(window);}
    window.display();
  }
  for (auto e:v) {delete e;}
}

void testGameEngine () {
  sf::ContextSettings anti_aliased;
  anti_aliased.antialiasingLevel = 4;
  sf::RenderWindow window (sf::VideoMode(500,500),"GameEngineTest",
                           sf::Style::None, anti_aliased);
  sf::Font font;
  if (!font.loadFromFile("font.otf")) {
    std::cerr<<"[ ERROR ] Cannot load font!\n";
    exit(EXIT_FAILURE);
  }
  { GameEngine game (window,font, 500,500); }
  window.close();
}

#ifdef __TESZTELUNK__

int main () {
  //testVektor();
  //testBox();
  //testEgyenes();
  testQTree2();
  testLaserMetszes();
  //testQTreeSize();
  //testQTree();
  //testQTreeFindLeaf();
  //testKocka();
  //testIntersects();
  //testIsect1();
  testGameEngine();
  return 0;
}

#endif
