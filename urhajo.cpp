#include "urhajo.hpp"

Urhajo::Urhajo (QTree<Hittable> *t, std::vector<Laser*> *ll, Vektor p,
                Vektor ir, int elet_, sf::Color col)
 : Hittable(t,Box({-3,-3},{-1,-1})), felrobbant{false},
   elet{elet_}, colour{col}, laserLista{ll} {
  csucslista.resize(3);
  setPos(p,ir);
}

Urhajo::~Urhajo () {
}

void Urhajo::draw(sf::RenderWindow& w) const {
  if (felrobbant) {
    sf::CircleShape sh (UrhajoSzel);
    sh.setFillColor(sf::Color::Red);
    sh.setPosition(getPos().x,getPos().y);
    w.draw(sh);
  } else {
    sf::VertexArray vertex_lista (sf::Triangles,3);
    for (size_t i = 0; i<3; ++i) {
      vertex_lista[i].position = sf::Vector2f(csucslista[i].x,csucslista[i].y);
      vertex_lista[i].color = colour;
    }
    w.draw(vertex_lista);
  }
}

void Urhajo::setPos (Vektor p, Vektor ir) {
  pos = p;
  irany = ir;
  if (!felrobbant) {
    csucslista[0] = pos;
    Vektor n {irany.y,-1*irany.x};
    csucslista[1] = pos - UrhajoHossz * irany + UrhajoSzel * n;
    csucslista[2] = pos - UrhajoHossz * irany - UrhajoSzel * n;
    Vektor m = csucslista[0], max = csucslista[0];
    for (size_t i = 1; i<3; ++i) {
      if (csucslista[i].x<m.x) {m.x = csucslista[i].x;}
      if (csucslista[i].y<m.y) {m.y = csucslista[i].y;}
      if (csucslista[i].x>max.x) {max.x = csucslista[i].x;}
      if (csucslista[i].y>max.y) {max.y = csucslista[i].y;}
    }
    Box ujhely (m,max);
    move(ujhely);
  } else {
    csucslista.clear();
  }
}

void Urhajo::hit () {
  elet = 0;
  felrobbant = true;
  tree->erase(this);
}

void Urhajo::hit (int sebzes) {
  if (elet-sebzes<=0) {
    hit();
  } else {
    elet -= sebzes;
  }
}

void Urhajo::loLasert () {
  Laser *l = new Laser (laserLista,tree, pos, irany, LaserKezdetiSebzes);
  laserLista->push_back(l);
}

