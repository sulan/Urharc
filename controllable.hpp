#ifndef CONTROLLABLE_HPP
#define CONTROLLABLE_HPP

#include "mestint.hpp"
#include "urhajo.hpp"

class GameEngine;

class Ellen : public Urhajo {
public:
  Ellen (GameEngine* e, AI *ai_, Vektor kpos, Vektor kirany);
  virtual ~Ellen ();
  virtual void update (int elapsed_time);
  virtual bool meghalt () const
    { return meghalt_counter<=0; }
protected:
  AI* ai;
  GameEngine *engine;
  int meghalt_counter;
private:
};

#endif
