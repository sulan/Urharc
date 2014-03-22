#ifndef LASER_HPP
#define LASER_HPP

#include "sprite.hpp"
#include "grafikus_rutinok.hpp"
#include "hittable.hpp"
#include <vector>

constexpr int LaserKezdetiSebzes = 10;
constexpr int LaserVegSebzes     = 2;

class Laser : public Sprite {
public:
  Laser (std::vector<Laser*> *p, QTree<Hittable> *t, Vektor kezd, Vektor ir,
         int seb);
  virtual ~Laser ();
  void sugarozz ();
  virtual void draw (sf::RenderWindow&) const;
protected:
  Vektor kezdopont;
  Vektor irany;
  Vektor vegpont;
  int sebzes;
  std::vector<Laser*> *parent;
  QTree<Hittable> *tree;
private:
};

#endif

