#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "keyboardInputHandler.h"

struct GameSettings;
class Snake;
class Heart;

class Game {
public:
  Game(const std::shared_ptr<GameSettings>& settings, const sf::Font& font);
  ~Game();
  void handleEvent(const sf::Event& event);
  void draw(sf::RenderWindow& window) const;
  KeyboardInputHandler& keyboardInputHandler();
  const sf::Texture& spriteSheet() const;
  bool tick();
  const std::shared_ptr<GameSettings>& settings() const;


private:
  std::vector<std::unique_ptr<Snake>> loadSnakes(KeyboardInputHandler& keyboardInputHandler);
  const std::unique_ptr<Heart>& collidesWithHeart(const Snake& snake) const;
  void addHeart();

  sf::Text _statusText;

  sf::Text _scoreText;
  int _heartsToProcess;
  unsigned int _score;
  const std::shared_ptr<GameSettings> _settings;
  KeyboardInputHandler _keyboardInputHandler;
  std::vector<std::unique_ptr<Snake>> _snakes;
  std::vector<std::unique_ptr<Heart>> _hearts;
  sf::Texture _spriteSheet;
};