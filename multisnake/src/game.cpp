#include "game.h"

#include "heart.h"
#include "snake.h"
#include "gameSettings.h"

Game::Game(const std::shared_ptr<GameSettings>& settings, const sf::Font& font) :
  _score(0),
  _heartsToProcess(0),
  _settings(settings) {
  _spriteSheet.loadFromFile("img/snake_spritesheet.png");
  _snakes = loadSnakes(_keyboardInputHandler);

  _scoreText = sf::Text("", font);
  _scoreText.setFillColor(sf::Color::Black);
  _scoreText.setOutlineColor(sf::Color::Black);

  while (_hearts.size() < _snakes.size()) {
    addHeart();
  }
}

Game::~Game() = default;

std::vector<std::unique_ptr<Snake>> Game::loadSnakes(KeyboardInputHandler& keyboardInputHandler) {
  std::vector<std::unique_ptr<Snake>> snakes;
  float offset = (float)_settings->cellSizeInPixels / 2;
  for (const auto& snakeConfig : _settings->snakes) {
    snakes.push_back(std::make_unique<Snake>(
      snakeConfig,
      *this)
    );
  }
  return snakes;
}

void Game::handleEvent(const sf::Event & event) {
  if (event.type == sf::Event::KeyPressed) {
    _keyboardInputHandler.onKeyDown(event.key);
  }
}

void Game::addHeart() {
  float randomX;
  float randomY;
  int tries = 0;
  float offset = (float)_settings->cellSizeInPixels / 2;
  do {
    randomX = (rand() % _settings->gameWidthInCells) * _settings->cellSizeInPixels + offset;
    randomY = (rand() % _settings->gameHeightInCells) * _settings->cellSizeInPixels + offset;
    tries++;
  } while (tries < 10 && std::any_of(_snakes.begin(), _snakes.end(), [randomX, randomY](const auto& snake) {
    auto position = sf::Vector2f(randomX, randomY);
    return snake->collidingWith(position) || snake->willCollideWithHead(position);
  }));
  
  _hearts.push_back(std::make_unique<Heart>(sf::Vector2f(randomX, randomY), *this));
}

const std::unique_ptr<Heart>& Game::collidesWithHeart(const Snake& snake) const {
  static std::unique_ptr<Heart> nullHack = nullptr;
  for (const auto& heart : _hearts) {
    if (snake.willCollideWithHead(heart->getPosition())) {
      return heart;
    }
  }

  return nullHack;
}

bool Game::tick() {
  for (auto& snake : _snakes) {
    auto& heart = collidesWithHeart(*snake);
    if (heart) {
      _score++;
      _hearts.erase(std::find(_hearts.begin(), _hearts.end(), heart));
      _heartsToProcess++;
    }
  }

  for (auto& snake : _snakes) {
    snake->moveForward(_heartsToProcess > 0);
  }

  for (auto& snake : _snakes) {
    for (unsigned int j = 0; j < _snakes.size(); ++j) {
      if (snake->collidingWith(*_snakes[j])) {
        return false;
      }
    }

    if (snake->outOfBounds()) {
      return false;
    }
  }
  
  _heartsToProcess = std::max(0, _heartsToProcess - 1);
  while (_hearts.size() < _snakes.size()) {
    addHeart();
  }

  _scoreText.setString("Score: " + std::to_string(_score));
  return true;
}

void Game::draw(sf::RenderWindow & window) const {
  for (const auto& snake : _snakes) {
    snake->draw(window);
  }

  for (const auto& heart : _hearts) {
    heart->draw(window);
  }

  window.draw(_scoreText);
  window.draw(_statusText);
}

KeyboardInputHandler& Game::keyboardInputHandler() {
  return _keyboardInputHandler;
}

const sf::Texture& Game::spriteSheet() const {
  return _spriteSheet;
}

const std::shared_ptr<GameSettings>& Game::settings() const {
  return _settings;
}
