#include "tukor.hpp"

Tukor::Tukor (QTree<Hittable> *t, Vektor a, Vektor b)
 : Hittable(t,Box({-3,-3},{-1,-1})), vertex_lista(sf::Quads,4) {
  csucslista.resize(4);
  csucslista[0] = a;
  csucslista[1] = b;
  Vektor n {(b-a).y,-1*(b-a).x};
  csucslista[2] = b+n;
  csucslista[3] = a+n;
  vertex_lista[0].position = sf::Vector2f(csucslista[0].x,csucslista[0].y);
  vertex_lista[1].position = sf::Vector2f(csucslista[1].x,csucslista[1].y);
  vertex_lista[2].position = sf::Vector2f(csucslista[2].x,csucslista[2].y);
  vertex_lista[3].position = sf::Vector2f(csucslista[3].x,csucslista[3].y);
  vertex_lista[0].color = sf::Color(180,180,180);
  vertex_lista[1].color = sf::Color(180,180,180);
  vertex_lista[2].color = sf::Color(180,180,180);
  vertex_lista[3].color = sf::Color(180,180,180);
  Vektor m = csucslista[0], max = csucslista[0];
  for (size_t i = 1; i<4; ++i) {
    if (csucslista[i].x<m.x) {m.x = csucslista[i].x;}
    if (csucslista[i].y<m.y) {m.y = csucslista[i].y;}
    if (csucslista[i].x>max.x) {max.x = csucslista[i].x;}
    if (csucslista[i].y>max.y) {max.y = csucslista[i].y;}
  }
  Box ujhely (m,max);
  move(ujhely);
}

Tukor::~Tukor () {
}
