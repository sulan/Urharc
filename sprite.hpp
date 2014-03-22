#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SFML/Graphics.hpp>

class Sprite
{
  public:
    Sprite() { }
    virtual ~Sprite() {  }
    virtual void draw (sf::RenderWindow&) const = 0;
  protected:
  private:
};

#endif // SPRITE_HPP
