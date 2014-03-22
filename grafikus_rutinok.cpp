#include "grafikus_rutinok.hpp"

bool intersect (const Box& a, const Box& b) {
  return (a.bal()<b.jobb()
        && a.jobb()>b.bal()
        && a.felso()<b.also()
        && a.also()>b.felso());
}

bool parhuzamos (Vektor a, Vektor b) {
  return std::abs(atan(a.y/a.x)-atan(b.y/b.x))<EPSILON;
}

bool parhuzamos (Egyenes a, Egyenes b) {
  return (std::abs(a.irany()-b.irany())<EPSILON
          || std::abs(std::abs(a.irany()-b.irany())-PI)<EPSILON);
}

