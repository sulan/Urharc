#ifndef MESTINT_HPP
#define MESTINT_HPP

#include "grafikus_rutinok.hpp"


class Player;
class Ellen;

class AI {
public:
  AI ();
  virtual ~AI ();
  virtual int kezdoElet () const = 0;
  virtual void update (int elapsed_time, Vektor& ujpos,
                       Vektor& ujirany,bool& lojj)      = 0;
  void setParent (Ellen* p) {
    parent = p;
  }
protected:
  const Ellen* parent;
private:
};

class ZombiTorpe : public AI {
public:
  ZombiTorpe(const Player* p);
  virtual ~ZombiTorpe ();
  virtual int kezdoElet() const
    { return 10; }
  virtual void update (int elapsed_time, Vektor& ujpos,
                       Vektor& ujirany, bool& lojj);
protected:
private:
  const Player *player;
};

class Hegy : public AI {
public:
  Hegy ();
  virtual ~Hegy ();
  virtual int kezdoElet() const
    { return 150; }
  virtual void update (int elapsed_time, Vektor& ujpos,
                       Vektor& ujirany, bool& lojj);
};

class Rolo : public AI {
public:
  Rolo () : counter{0} {}
  virtual ~Rolo () {}
  virtual int kezdoElet() const {return 50;}
  virtual void update (int elapsed_time, Vektor& ujpos,
                       Vektor& ujirany, bool& lojj);
private:
  int counter;
};

#endif
