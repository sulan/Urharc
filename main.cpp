#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "game_engine.hpp"
#include "grafikus_rutinok.hpp"

using namespace std;

constexpr double XX = 1000;
constexpr double YY = 720;

void printSzoveg (string szoveg, sf::RenderWindow& w, sf::Font f, Vektor hova,
                  unsigned size) {
  sf::Text t (szoveg,f,size);
  t.setPosition(hova.x,hova.y);
  t.setColor(sf::Color::White);
  w.draw(t);
}

void drawMenu (sf::RenderWindow& w, sf::Font& f) {
  printSzoveg("URHARC JATEK",w,f,{435,250},30);
  printSzoveg("(adatszerkezetek nagybeadando)",w,f,{392,300},20);
  printSzoveg("Nyomj ENTER-t a kezdeshez",w,f,{395,350},25);
  printSzoveg("Programozta:",w,f,{1,650},15);
  printSzoveg("sulan",w,f,{120,650},15);
  printSzoveg("Quality Assurance:",w,f,{1,670},16);
  printSzoveg("Hunyady Marton, Kozak Csaba es Polcz Peter",
              w,f,{120,670},16);
}

void menuLoop () {
  sf::ContextSettings anti_aliased;
  anti_aliased.antialiasingLevel = 4;
  sf::RenderWindow window (sf::VideoMode(XX,YY),"GameEngineTest",
                           sf::Style::None, anti_aliased);
  sf::Font font;
  if (!font.loadFromFile("font.otf")) {
    std::cerr<<"[ ERROR ] Cannot load font!\n";
    exit(EXIT_FAILURE);
  }
  while (window.isOpen()) {
    sf::Event ev;
    while (window.pollEvent(ev)) {
      if (ev.type == sf::Event::Closed
		        || (ev.type == sf::Event::KeyPressed
		        	&& ev.key.code==sf::Keyboard::Escape)) {
        window.close();
      } else
      if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::Return) {
          GameEngine engine (window,font,XX,YY);
        }
      }
    }
    window.clear();
    drawMenu(window,font);
    window.display();
  }
}

int main () {
  menuLoop();
  return 0;
}
