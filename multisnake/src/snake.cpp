#include "snake.h"

#include "keyboardInputHandler.h"
#include "game.h"

Snake::Snake(const CreateSnakeParameters& parameters, Game& game) : 
  _inputDirection(parameters.velocity),
  _velocity(parameters.velocity),
  _bodyParts(),
  _color(parameters.color),
  _game(game) {
  _bodyParts.push_back(createHead(parameters.startPosition));
  game.keyboardInputHandler().registerInput(parameters.downKey, std::bind(&Snake::changeDirection, this, sf::Vector2f(0, 32)));
  game.keyboardInputHandler().registerInput(parameters.upKey, std::bind(&Snake::changeDirection, this, sf::Vector2f(0, -32)));
  game.keyboardInputHandler().registerInput(parameters.leftKey, std::bind(&Snake::changeDirection, this, sf::Vector2f(-32, 0)));
  game.keyboardInputHandler().registerInput(parameters.rightKey, std::bind(&Snake::changeDirection, this, sf::Vector2f(32, 0)));
}

void Snake::changeDirection(const sf::Vector2f& direction) {
  if ((_velocity + direction).x != 0 && (_velocity + direction).y != 0) {
    _inputDirection = direction;
  }
}

void Snake::draw(sf::RenderWindow & window) const {
  for (const auto& bodyPart : _bodyParts) {
    bodyPart->draw(window);
  }
}

std::unique_ptr<SnakeBody> Snake::createHead(const sf::Vector2f& position) {
  float angle = atan2(_velocity.y, _velocity.x) * (float)(180 / 3.14) + 90;
  return std::make_unique<SnakeBody>(_game.spriteSheet(), _game.settings(), position, _color, angle);
}

void Snake::moveForward(bool gotHeart) {
  _velocity = _inputDirection;
  auto& head = _bodyParts.front();
  head->makeBody();

  if (gotHeart) {
    _bodyParts.push_front(createHead(head->getPosition() + _velocity));
  } else {
    auto& end = _bodyParts.back();
    float angle = atan2(_velocity.y, _velocity.x) * (float)(180 / 3.14) + 90;
    end->makeHead(head->getPosition() + _velocity, angle);
    _bodyParts.push_front(std::move(end));
    _bodyParts.pop_back();
  }
}

bool Snake::collidingWith(const Snake& snake) const {
  for (unsigned int i = &snake == this ? 1 : 0; i < snake._bodyParts.size(); ++i) {
    if (headCollidingWith(snake._bodyParts[i]->getPosition())) {
      return true;
    }
  }
  return false;
}

bool Snake::collidingWith(const sf::Vector2f& position) const {
  for (unsigned int i = 0; i < _bodyParts.size(); ++i) {
    if (position == _bodyParts[i]->getPosition()) {
      return true;
    }
  }
  return false;
}

bool Snake::headCollidingWith(const sf::Vector2f& position) const {
  const auto& headPosition = _bodyParts[0]->getPosition();
  return headPosition == position;
}

bool Snake::willCollideWithHead(const sf::Vector2f& position) const {
  const auto& futureHeadPosition = _bodyParts[0]->getPosition() + _inputDirection;
  return futureHeadPosition == position;
}

bool Snake::outOfBounds() const {
  const auto& headPosition = _bodyParts[0]->getPosition();
  return (headPosition.x < 0) ||
    (headPosition.x > _game.settings()->gameWidthInCells * _game.settings()->cellSizeInPixels) ||
    (headPosition.y < 0) ||
    (headPosition.y > _game.settings()->gameHeightInCells * _game.settings()->cellSizeInPixels);
}
