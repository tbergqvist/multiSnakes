#pragma once
#include<string>
#include<deque>
#include <SFML/Graphics.hpp>
#include "gameSettings.h"

class Game;

class SnakeBody {
public:
  SnakeBody(const sf::Texture& spriteSheet, const std::shared_ptr<GameSettings>& gameSettings, const sf::Vector2f& position, const sf::Color& color, const float rotationAngle) :
    _gameSettings(gameSettings) {
    _sprite = sf::Sprite(spriteSheet);
    _sprite.setColor(color);
    float halfCellSize = (float)gameSettings->cellSizeInPixels / 2;
    _sprite.setOrigin(halfCellSize, halfCellSize);

    _noColorSprite = sf::Sprite(spriteSheet);
    _noColorSprite.setOrigin(halfCellSize, halfCellSize);

    makeHead(position, rotationAngle);
  }

  void draw(sf::RenderWindow& window) {
    window.draw(_sprite);
    window.draw(_noColorSprite);
  }

  const sf::Vector2f& getPosition() {
    return _sprite.getPosition();
  }

  const float getRotation() {
    return _sprite.getRotation();
  }

  void makeHead(const sf::Vector2f& position, const float rotationAngle) {
    int cellSize = _gameSettings->cellSizeInPixels;
    _sprite.setPosition(position);
    _sprite.setRotation(rotationAngle);
    _sprite.setTextureRect(sf::IntRect(0, 0, cellSize, cellSize));

    _noColorSprite.setPosition(position);
    _noColorSprite.setRotation(rotationAngle);
    _noColorSprite.setTextureRect(sf::IntRect(0, cellSize, cellSize, cellSize));
  }
  
  void makeBody() {
    int cellSize = _gameSettings->cellSizeInPixels;
    _sprite.setTextureRect(sf::IntRect(cellSize, 0, cellSize, cellSize));
    _noColorSprite.setTextureRect(sf::IntRect(cellSize, cellSize, cellSize, cellSize));
  }

private:
  std::shared_ptr<GameSettings> _gameSettings;
  sf::Sprite _sprite;
  sf::Sprite _noColorSprite;
};

class Snake {
public:
  Snake(const CreateSnakeParameters& parameters, Game& game);
  void changeDirection(const sf::Vector2f& direction);
  void draw(sf::RenderWindow& window) const;
  void moveForward(bool gotHeart);
  bool collidingWith(const Snake& snake) const;
  bool collidingWith(const sf::Vector2f& position) const;
  bool willCollideWithHead(const sf::Vector2f& position) const;
  bool outOfBounds() const;
private:
  bool headCollidingWith(const sf::Vector2f& position) const;
  std::unique_ptr<SnakeBody> createHead(const sf::Vector2f& position);

  Game& _game;

  std::deque<std::unique_ptr<SnakeBody>> _bodyParts;
  sf::Vector2f _velocity;
  sf::Vector2f _inputDirection;
  sf::Color _color;
};
