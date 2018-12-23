#include "heart.h"
#include "game.h"

Heart::Heart(const sf::Vector2f & startPosition, const Game& game) {
  _sprite = sf::Sprite(game.spriteSheet(), sf::IntRect(64, 0, 32, 32));
  _sprite.setOrigin(16, 16);
  _sprite.setPosition(startPosition);
}

void Heart::draw(sf::RenderWindow & window) const {
  window.draw(_sprite);
}

sf::Vector2f Heart::getPosition() const {
  return _sprite.getPosition();
}