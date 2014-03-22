#ifndef URHAJO_HPP
#define URHAJO_HPP

#include "sprite.hpp"
#include "hittable.hpp"
#include "laser.hpp"

constexpr double UrhajoHossz = 20;
constexpr double UrhajoSzel  = 5;

class Urhajo : public Sprite, public Hittable {
public:
  Urhajo (QTree<Hittable>* t, std::vector<Laser*> *ll, Vektor p, Vektor ir,
          int elet_, sf::Color col);
  virtual ~Urhajo ();
  virtual void draw(sf::RenderWindow&) const;
  virtual void hit ();
  virtual void hit (int sebzes);
  Hittable::Tipus getTipus () const final
    { return Hittable::Tipus::Urhajo; }
  Vektor getPos () const
    { return pos; }
  Vektor getIrany () const
    { return irany; }
protected:
  bool felrobbant;
  int elet;
  //sf::Texture *texture;
  sf::Color colour;
  std::vector<Laser*> *laserLista;
  void setPos (Vektor p, Vektor ir);
  void loLasert ();
private:
  Vektor pos;
  Vektor irany;
};

#endif
