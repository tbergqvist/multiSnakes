#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <functional>
#include <SFML/Window/Event.hpp>

class KeyboardInputHandler {
public:
  void onKeyDown(const sf::Event::KeyEvent& event);
  void registerInput(const std::string& input, std::function<void()> callback);

private:
  std::map<sf::Keyboard::Key, std::vector<std::function<void()>>> _registeredInputs;
};
