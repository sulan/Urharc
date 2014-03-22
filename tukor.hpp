#ifndef TUKOR_HPP
#define TUKOR_HPP

#include "hittable.hpp"
#include "sprite.hpp"

class Tukor : public Hittable, public Sprite {
public:
  Tukor (QTree<Hittable> *t, Vektor a, Vektor b);
  virtual ~Tukor ();
  virtual void draw (sf::RenderWindow& w) const {
    w.draw(vertex_lista);
  }
  virtual void hit () {}
  virtual void hit (int) {}
  Hittable::Tipus getTipus () const final
    { return Hittable::Tipus::Tukor; }
protected:
  sf::VertexArray vertex_lista;
private:
};

#endif
