#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>
#include <map>
#include "game.h"

#include "gameSettings.h"

enum GameState {
  NOT_STARTED,
  RUNNING,
  GAME_OVER
};

std::string getStatusString(GameState state) {
  switch (state) {
  case NOT_STARTED:
    return "Press SPACE to start!";
  case RUNNING:
    return "";
  case GAME_OVER:
    return "          Game Over! \n Press SPACE to restart!";
  }

  return "";
}

std::shared_ptr<GameSettings> parseConfig(const std::string& filename) {
  std::ifstream is(filename);
  std::string name;
  std::string value;
  std::vector<std::map<std::string, std::string>> snakeValues;
  std::map<std::string, std::string> settingsMap;

  std::map<std::string, std::string>* currentMap = &settingsMap;

  while (is >> name >> value) {
    if (name == "player") {
      std::map<std::string, std::string> playerMap;
      snakeValues.push_back(playerMap);
      currentMap = &snakeValues.back();
      continue;
    }
    (*currentMap)[name] = value;
  }

  std::vector<CreateSnakeParameters> createSnakeParameters;
  for (const auto& snakeSetting : snakeValues) {
    createSnakeParameters.push_back(CreateSnakeParameters{
      snakeSetting.at("upKey"),
      snakeSetting.at("downKey"),
      snakeSetting.at("leftKey"),
      snakeSetting.at("rightKey"),
      sf::Vector2f(std::stof(snakeSetting.at("positionX")), std::stof(snakeSetting.at("positionY"))),
      sf::Vector2f(std::stof(snakeSetting.at("velocityX")), std::stof(snakeSetting.at("velocityY"))),
      sf::Color((unsigned int)std::stoll(snakeSetting.at("color"), 0, 16))
    });
  }

  return std::make_shared<GameSettings>(
    GameSettings{
      static_cast<unsigned int>(std::stoi(settingsMap.at("tileSize"))),
      static_cast<unsigned int>(std::stoi(settingsMap.at("screenWidth"))),
      static_cast<unsigned int>(std::stoi(settingsMap.at("screenHeight"))),
      createSnakeParameters
    });
  ;
}

int main()
{
  const auto settings = parseConfig("config.txt");
  srand((unsigned int)time(0));
  int currentFrames = 0;
  sf::Clock clock;
  GameState gameState = NOT_STARTED;

  sf::Font font;
  font.loadFromFile("fonts/LiberationSans-Regular.ttf");
  sf::Text statusText = sf::Text("", font);
  statusText.setFillColor(sf::Color::Black);
  statusText.setOutlineColor(sf::Color::Black);
  statusText.setPosition(sf::Vector2f(100, 200));

  sf::RenderWindow window(sf::VideoMode(settings->cellSizeInPixels * settings->gameWidthInCells, settings->cellSizeInPixels * settings->gameHeightInCells), "Snake game!");
  window.setVerticalSyncEnabled(true);
  std::unique_ptr<Game> game = std::make_unique<Game>(settings, font);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      
      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        if (gameState == NOT_STARTED) {
          gameState = RUNNING;
        } else if (gameState == GAME_OVER) {
          gameState = RUNNING;
          game = std::make_unique<Game>(settings, font);
        }
      }

      if (gameState == RUNNING) {
        game->handleEvent(event);
      }

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    if (gameState == GameState::RUNNING && clock.getElapsedTime().asSeconds() > 0.1) {
      clock.restart();
      if (!game->tick()) {
        gameState = GameState::GAME_OVER;
      }
      
    }
    statusText.setString(getStatusString(gameState));

    window.clear(sf::Color::White);
    game->draw(window);
    window.draw(statusText);
    window.display();
    currentFrames++;
  }

  return 0;
}