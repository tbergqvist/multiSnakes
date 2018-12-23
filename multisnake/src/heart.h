#pragma once
#include<string>
#include <SFML/Graphics.hpp>

class Game;

class Heart {
public:
  Heart(const sf::Vector2f& position, const Game& game);
  void draw(sf::RenderWindow& window) const;
  sf::Vector2f getPosition() const;
private:
  sf::Sprite _sprite;
};
