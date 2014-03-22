#include "game_engine.hpp"
#include <string>
#include <ctime>
#include <random>


#ifdef _WIN32
#include <sstream>
std::string to_string(int a) {
  stringstream ss{};
  ss<<a;
  return ss.str();
}
#else
using std::to_string;
#endif

GameEngine::GameEngine (sf::RenderWindow& w, sf::Font f, double width, double height)
 : XX{width}, YY{height-20}, window(w), font{f}, tree(Box({0,0},{XX,YY}))  {
  std::minstd_rand eng (time(NULL));
  //Letrehozzuk az elemeket
  player = new Player(this);
  /*
  tukorLista.push_back(new Tukor(&tree,{100,100},{150,150}));
  tukorLista.push_back(new Tukor(&tree,{400,50},{400,100}));
  tukorLista.push_back(new Tukor(&tree,{400,450},{400,499}));
  tukorLista.push_back(new Tukor(&tree,{300,450},{300,499}));

  ellenLista.push_back(new Ellen(this,new ZombiTorpe(player),{0,250},{1,1}));
  ellenLista.push_back(new Ellen(this,new ZombiTorpe(player),{0,260},{1,1}));
  ellenLista.push_back(new Ellen(this,new ZombiTorpe(player),{0,270},{1,1}));
  ellenLista.push_back(new Ellen(this,new ZombiTorpe(player),{0,0},{1,1}));
  ellenLista.push_back(new Ellen(this,new Hegy(),{450,255},{-1,1}));
  Ellen *e = new Ellen(this,new Rolo(),{190,500},{0,-1});
  if (beprobal(e)) { ellenLista.push_back(e); } */


  //Tukor
  for (int i = 0; i<14; ++i) {
    std::uniform_real_distribution<double> ex (0,XX);
    std::uniform_real_distribution<double> ey (0,YY);
    Vektor pos {ex(eng),ey(eng)};
    if (Box({XX/2-100,YY/2-100},{XX/2+100,YY/2+100}).in(pos)) { continue; }
    std::normal_distribution<double> mx (pos.x,50);
    std::normal_distribution<double> my (pos.y,50);
    Vektor mpos {mx(eng),my(eng)};
    if ((pos-mpos).abs()<20) { continue; }
    Tukor* t = new Tukor(&tree,pos,mpos);
    if (beprobal(t)) { tukorLista.push_back(t); }
    else {delete t;}
  }

  //Zombitorpe
  for (int i = 0; i<5; ++i) {
    std::uniform_real_distribution<double> ex (0,XX);
    std::uniform_real_distribution<double> ey (0,YY);
    std::uniform_real_distribution<double> ix (-1,1);
    std::uniform_real_distribution<double> iy (-1,1);
    Vektor irany = {ix(eng),iy(eng)}; irany *= 1.0/irany.abs();
    Ellen * e = new Ellen (this,new ZombiTorpe(player),{ex(eng),ey(eng)},
                                                       irany);
    if (beprobal(e)) {ellenLista.push_back(e);}
    else { delete e; }
  }

  //Hegy
  for (int i = 0; i<2; ++i) {
    std::uniform_real_distribution<double> ex (0,XX);
    std::uniform_real_distribution<double> ey (0,YY);
    std::uniform_real_distribution<double> ix (-1,1);
    std::uniform_real_distribution<double> iy (-1,1);
    Vektor irany = {ix(eng),iy(eng)}; irany *= 1.0/irany.abs();
    Ellen * e = new Ellen (this,new Hegy,{ex(eng),ey(eng)},
                                                       irany);
    if (beprobal(e)) {ellenLista.push_back(e);}
    else { delete e; }
  }

  //Rolo
  for (int i = 0; i<4; ++i) {
    std::uniform_real_distribution<double> ex (0,XX);
    std::uniform_real_distribution<double> ey (0,YY);
    std::uniform_real_distribution<double> ix (-1,1);
    std::uniform_real_distribution<double> iy (-1,1);
    Vektor irany = {ix(eng),iy(eng)}; irany *= 1.0/irany.abs();
    Ellen * e = new Ellen (this,new Rolo,{ex(eng),ey(eng)},
                                                       irany);
    if (beprobal(e)) {ellenLista.push_back(e);}
    else { delete e; }
  }

  //Mehet a jatek
  run();
}

GameEngine::~GameEngine () {
  delete player;
  for (auto e:tukorLista) { delete e; }  tukorLista.clear();
  for (auto e:laserLista) { delete e; }  laserLista.clear();
  for (auto e:ellenLista) { delete e; }  ellenLista.clear();
}

void GameEngine::run() {
  auto most = std::chrono::high_resolution_clock::now();
  auto old_most = most;
  while (window.isOpen() && !player->meghalt() && !ellenLista.empty()) {
    sf::Event ev;
    while (window.pollEvent(ev)) {
      if ( ev.type == sf::Event::Closed ||
           (ev.type == sf::Event::KeyPressed &&
             ev.key.code == sf::Keyboard::Escape) ) {
        //window.close();
        return ;
      }
      //if (ev.type == sf::Event::KeyPressed) {
      //  player->update(ev);
      //}
    }
    most = std::chrono::high_resolution_clock::now();
    int elapsed_time
      = std::chrono::duration_cast<std::chrono::milliseconds>
                                           (most-old_most).count();
    if (elapsed_time >= SEB) {
      kor(elapsed_time);
      old_most = most;
    }
  }
  if (window.isOpen() && player->meghalt()) {
    sf::Text t ("MEGHALTAL", font, 30);
    t.setColor(sf::Color::White);
    t.setPosition(XX/2-50,YY/2-15);
    window.draw(t);
    window.display();
    sf::Event ev;
    while (window.waitEvent(ev)) {
      if ( ev.type == sf::Event::Closed ||
           (ev.type == sf::Event::KeyPressed &&
            (ev.key.code == sf::Keyboard::Escape ||
             ev.key.code == sf::Keyboard::Return)) ) {
        return ;
      }
    }
  }
  if (window.isOpen() && ellenLista.empty()) {
    sf::Text t ("SZEP VOLT", font, 30);
    t.setColor(sf::Color::White);
    t.setPosition(XX/2-50,YY/2-15);
    window.draw(t);
    window.display();
    sf::Event ev;
    while (window.waitEvent(ev)) {
      if ( ev.type == sf::Event::Closed ||
           (ev.type == sf::Event::KeyPressed &&
            (ev.key.code == sf::Keyboard::Escape ||
             ev.key.code == sf::Keyboard::Return)) ) {
        return ;
      }
    }
  }
}

void GameEngine::kor (int elapsed_time) {
  updateAll(elapsed_time);
  collisionDetection();
  calcLasers();
  hullaCollection();
  drawAll();
}

void GameEngine::updateAll(int elapsed_time) {
  player->update(elapsed_time);
  for (Ellen* e:ellenLista) {
    e->update(elapsed_time);
  }
}

void GameEngine::collisionDetection() {
  player->detectCollision();
  for (Ellen* e:ellenLista) {
    e->detectCollision();
  }
}

void GameEngine::calcLasers() {
  size_t i = 0;
  while (i<laserLista.size()) {
    laserLista[i++]->sugarozz();
  }
}

void GameEngine::hullaCollection() {
  for (size_t i = 0; i<ellenLista.size(); ++i) {
    if (ellenLista[i]->meghalt()) {
      delete ellenLista[i];
      ellenLista[i] = ellenLista[ellenLista.size()-1];
      ellenLista.pop_back();
    }
  }
}

void GameEngine::drawAll() {
  window.clear();
  for (Ellen* e:ellenLista) { e->draw(window); }
  for (Tukor* t:tukorLista) { t->draw(window); }
  for (Laser* l:laserLista) { l->draw(window); delete l; }
  laserLista.clear();
  player->draw(window);
  drawStatusBar();
  window.display();
}

void GameEngine::drawStatusBar() {
  //double real_height = YY+20;
  sf::RectangleShape bar (sf::Vector2f(XX,20));
  bar.setPosition(0,YY);
  bar.setFillColor(sf::Color::White);
  window.draw(bar);
  std::string elet ("Elet ");
  elet += to_string(player->getElet());
  std::string lazer ("Lezer toltottsege: ");
  lazer += to_string(player->getLaserToltottseg());
  lazer += "%";
  sf::Text text (elet,font,15);
  text.setColor(sf::Color::Black);
  text.setPosition(5,YY+2);
  window.draw(text);
  sf::Text laser_text (lazer,font,15);
  laser_text.setColor(sf::Color::Black);
  laser_text.setPosition(105,YY+2);
  window.draw(laser_text);
}

bool GameEngine::beprobal (Hittable *elem) {
  std::set<Hittable*> korny = tree.find(elem->box());
  for (Hittable* e:korny) {
    if (e!=elem && !e->getCsucslista().empty() && intersect(*elem,*e)) {
      return false;
    }
  }
  return true;
}



















