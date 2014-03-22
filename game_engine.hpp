#ifndef GAME_ENGINE_HPP
#define GAME_ENGINE_HPP

#include "controllable.hpp"
#include "laser.hpp"
#include "tukor.hpp"
#include "player.hpp"
#include <chrono>

constexpr int SEB = 40;

class GameEngine {
public:
  GameEngine(sf::RenderWindow& w, sf::Font f, double XX, double YY);
  ~GameEngine();
  QTree<Hittable> * getTree ()
    { return &tree; }
  std::vector<Laser*>* getLaserLista ()
    { return &laserLista; }
  Vektor getDim () const
    {return {XX,YY};}
protected:
  double XX, YY;
  sf::RenderWindow &window;
  sf::Font font;
  Player *player;
  std::vector<Tukor*> tukorLista {};
  std::vector<Ellen*> ellenLista {};
  std::vector<Laser*> laserLista {};
  QTree<Hittable> tree;
  void run ();
  void kor (int elapsed_time);
  void updateAll(int elapsed_time);
  void collisionDetection();
  void calcLasers();
  void hullaCollection();
  void drawAll();
  void drawStatusBar();
  bool beprobal (Hittable *elem);
private:
};

#endif
