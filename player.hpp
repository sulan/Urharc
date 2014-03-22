#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "urhajo.hpp"

constexpr double PlayerSeb      = 0.05;
constexpr int    PlayerElet     = 100;
constexpr int    PlayerLaserMax = 10;

class GameEngine;

class Player : public Urhajo {
public:
  Player (GameEngine* e);
  virtual ~Player ();
  //virtual void update (const sf::Event::KeyEvent& ev);
  virtual void update (int elapsed_time);
  virtual bool meghalt () const
    { return meghalt_counter<=0; }
  int getElet () const
    { return elet; }
  int getLaserToltottseg () const {
    return std::max(static_cast<int>(laser_counter*100/PlayerLaserMax),0);
  }
protected:
  GameEngine *engine;
  int meghalt_counter;
  double laser_counter;
private:
};

#endif
