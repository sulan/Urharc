#include "laser.hpp"

Laser::Laser (std::vector<Laser*> *p, QTree<Hittable> *t, Vektor kezd,
                Vektor ir, int seb)
 : kezdopont{kezd+ir*0.5},irany{ir}, vegpont{-1,-1}, sebzes{seb}, parent{p}, tree{t} {
}

Laser::~Laser () {
}

void Laser::draw(sf::RenderWindow& w) const {
  sf::VertexArray va (sf::Lines, 2);
  va[0].position = sf::Vector2f(kezdopont.x,kezdopont.y);
  va[0].color = sf::Color::Yellow;
  va[1].position = sf::Vector2f(vegpont.x,vegpont.y);
  va[1].color = sf::Color::Yellow;
  w.draw(va);
}

void Laser::sugarozz() {
  Vektor p = kezdopont;
  while (true) {
    auto leaf = tree->findLeaf(p);
    if (!leaf.second.in(p)) {
      break;
    }
    double min = INFTY;
    Hittable* minelem = nullptr;
    std::pair<Vektor,Vektor> minv;
    for (Hittable *e:leaf.first) {
      double t;
      std::vector<Vektor> v = e->getCsucslista();
      if (v.empty()) { continue; }
      for (size_t i = 1; i<v.size(); ++i) {
        t = laserMetszi(kezdopont,irany,v[i-1],v[i]);
        if (t>=0 && t<min) {
          min = t;
          minelem = e;
          minv = std::make_pair(v[i-1],v[i]);
        }
      }
      t = laserMetszi(kezdopont,irany,v[v.size()-1],v[0]);
      if (t>=0 && t<min) {
        min = t;
        minelem = e;
        minv = std::make_pair(v[v.size()-1],v[0]);
      }
    }
    if (minelem) {
      vegpont = kezdopont + min*irany;
      if (minelem->getTipus() == Hittable::Tipus::Tukor) {
        if (sebzes>LaserVegSebzes) {
          parent->push_back(
             new Laser(parent,tree, vegpont,
                        tukor(irany,minv.first,minv.second),sebzes/2)
          );
        }
      } else {
        minelem->hit(sebzes);
      }
      break;
    } else {
      min = INFTY;
      double t = laserMetszi(kezdopont,irany,leaf.second.bf(),leaf.second.jf());
      if (t>=0 && t<min && !rajta(kezdopont,p,kezdopont+t*irany)) { min = t; }
      t = laserMetszi(kezdopont,irany,leaf.second.jf(),leaf.second.ja());
      if (t>=0 && t<min && !rajta(kezdopont,p,kezdopont+t*irany)) { min = t; }
      t = laserMetszi(kezdopont,irany,leaf.second.ja(),leaf.second.ba());
      if (t>=0 && t<min && !rajta(kezdopont,p,kezdopont+t*irany)) { min = t; }
      t = laserMetszi(kezdopont,irany,leaf.second.ba(),leaf.second.bf());
      if (t>=0 && t<min && !rajta(kezdopont,p,kezdopont+t*irany)) { min = t; }
      if (min<INFTY) {
        vegpont = kezdopont+min*irany;
        p = kezdopont+(min+1)*irany;
      } else {
        break;
      }
    }
  }
}











