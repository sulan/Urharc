#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "qtree.hpp"
#include <vector>

class Hittable
{
  public:
    enum class Tipus {Tukor, Urhajo};
    Hittable(QTree<Hittable>* t, const Box& bound);
    virtual ~Hittable();
    const Box& box () const
      { return boundary; }
    /*Halalosan*/
    virtual void hit () = 0;
    /*Sebez*/
    virtual void hit (int sebzes) = 0;
    virtual void detectCollision();
    const std::vector<Vektor>& getCsucslista () const
      { return csucslista; }
    virtual Tipus getTipus () const = 0;
  protected:
    std::vector<Vektor> csucslista;
    QTree<Hittable>* tree;
    void move (Box ujhely);
  private:
    Box boundary;
};

inline bool intersect(const Hittable& a, const Hittable& b) {
  return intersect(a.getCsucslista(),b.getCsucslista());
}

#endif // HITTABLE_HPP
