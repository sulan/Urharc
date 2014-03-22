#include "mestint.hpp"
#include "controllable.hpp"
#include "player.hpp"
#include <cstdlib>
#include <ctime>
#include <random>

AI::AI () : parent{} {
}

AI::~AI () {
}

///Zombi-torpe

ZombiTorpe::ZombiTorpe (const Player* p)
     : AI{}, player{p} {
}

ZombiTorpe::~ZombiTorpe () {
}


void ZombiTorpe::update (int elapsed_time, Vektor& ujpos,
                  Vektor& ujirany, bool& lojj) {
  ujirany = player->getPos()-parent->getPos();
  ujirany *= 1.0/ujirany.abs();
  ujpos += ujirany*elapsed_time*0.04;
  lojj = false;
}


///Hegy
Hegy::Hegy () {
}

Hegy::~Hegy () {
}

void Hegy::update (int , Vektor& ,
                       Vektor& ujirany, bool& lojj) {
  std::minstd_rand e (time(NULL));
  std::normal_distribution<double> d (0,PI/55);
  ujirany = forgat(ujirany,d(e));
  lojj = true;
}

///Rolo
void Rolo::update (int , Vektor& ujpos,
                       Vektor& irany, bool& lojj) {
  lojj = true;
  Vektor n {irany.y,-1*irany.x};
  if (counter<0) {
    ujpos += n;
  } else {
    ujpos -= n;
  }
  if (++counter==30) {
    counter = -30;
  }
}
