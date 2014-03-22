#include "player.hpp"
#include "game_engine.hpp"
#include "grafikus_rutinok.hpp"

Player::Player (GameEngine* e)
 : Urhajo(e->getTree(),e->getLaserLista(),e->getDim()*0.5,{0,-1},PlayerElet,
   sf::Color::Green), engine{e}, meghalt_counter{5}, laser_counter{10} {
}

Player::~Player () {
}

void Player::update(int elapsed_time) {
  if (felrobbant) {
    if (meghalt_counter>0) { --meghalt_counter; }
    return;
  }
  Vektor ujpos = getPos(),ujirany = getIrany();
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    ujpos = getPos()+elapsed_time*PlayerSeb*getIrany();
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    ujirany = forgat(getIrany(),-0.05*PI);
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    ujirany = forgat(getIrany(),0.05*PI);
  }
  setPos(ujpos,ujirany);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {                        //laser
    if (laser_counter>0) {
      loLasert();
    }
    if (laser_counter>-2) { laser_counter-=1.0; }
  } else {
    if (laser_counter<PlayerLaserMax) {
      laser_counter+=0.6;
      if (laser_counter>PlayerLaserMax) { laser_counter = PlayerLaserMax; }
    }
  }
  if ( ujpos.x > engine->getDim().x+200 || ujpos.x < -200 ||                  //kiment a palyarol
         ujpos.y > engine->getDim().y+200 || ujpos.y < -200 ) {
    felrobbant = true;
    meghalt_counter = 0;
  }
}
