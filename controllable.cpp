#include "controllable.hpp"
#include "game_engine.hpp"

Ellen::Ellen (GameEngine* e, AI *ai_, Vektor kpos, Vektor kirany)
 : Urhajo(e->getTree(),e->getLaserLista(),kpos,kirany,ai_->kezdoElet(),sf::Color::Blue),
   ai{ai_}, engine{e},meghalt_counter{5}{
  ai->setParent(this);
}

Ellen::~Ellen () {
  delete ai;
}

void Ellen::update(int elapsed_time) {
  if (!felrobbant) {
    Vektor ujpos {getPos()}, ujirany {getIrany()};
    bool lojj;
    ai->update(elapsed_time, ujpos, ujirany, lojj);
    setPos(ujpos,ujirany);
    if ( ujpos.x > engine->getDim().x+200 || ujpos.x < -200 ||                  //kiment a palyarol
         ujpos.y > engine->getDim().y+200 || ujpos.y < -200 ) {
      felrobbant = true;
      meghalt_counter = 0;
    } else {
      if (lojj) {loLasert();}
    }
  } else {
    if (meghalt_counter>0) {
      --meghalt_counter;
    }
  }
}


