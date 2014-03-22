#include "hittable.hpp"

///===================Hittable====================

Hittable::Hittable(QTree<Hittable>* t, const Box& bound)
  : tree{t}, boundary{bound}
{
  tree->insert(this);
}

Hittable::~Hittable() {
  tree->erase(this);
}

void Hittable::move(Box ujhely) {
  tree->move(this,ujhely);
  boundary = ujhely;
}

void Hittable::detectCollision() {
  if (csucslista.empty()) { return; }
  std::set<Hittable*> korny = tree->find(boundary);
  for (Hittable* e:korny) {
    if (e!=this && !e->getCsucslista().empty() && intersect(*this,*e)) {
      hit();
      e->hit();
      //?break;
    }
  }
}
