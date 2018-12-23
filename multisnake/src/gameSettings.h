#pragma once

struct CreateSnakeParameters {
  const std::string upKey;
  const std::string downKey;
  const std::string leftKey;
  const std::string rightKey;

  const sf::Vector2f startPosition;
  const sf::Vector2f velocity;
  const sf::Color color;
};

struct GameSettings {
  const unsigned int cellSizeInPixels;
  const unsigned int gameWidthInCells;
  const unsigned int gameHeightInCells;

  const std::vector<CreateSnakeParameters> snakes;
};