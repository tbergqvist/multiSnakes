#include "keyboardInputHandler.h"

static sf::Keyboard::Key stringToSfmlKey(const std::string & str) {
  if ("A" == str) {
    return sf::Keyboard::Key::A;
  } else if ("B" == str) {
    return sf::Keyboard::Key::B;
  } else if ("C" == str) {
    return sf::Keyboard::Key::C;
  } else if ("D" == str) {
    return sf::Keyboard::Key::D;
  } else if ("E" == str) {
    return sf::Keyboard::Key::E;
  } else if ("F" == str) {
    return sf::Keyboard::Key::F;
  } else if ("G" == str) {
    return sf::Keyboard::Key::G;
  } else if ("H" == str) {
    return sf::Keyboard::Key::H;
  } else if ("I" == str) {
    return sf::Keyboard::Key::I;
  } else if ("J" == str) {
    return sf::Keyboard::Key::J;
  } else if ("K" == str) {
    return sf::Keyboard::Key::K;
  } else if ("L" == str) {
    return sf::Keyboard::Key::L;
  } else if ("M" == str) {
    return sf::Keyboard::Key::M;
  } else if ("N" == str) {
    return sf::Keyboard::Key::N;
  } else if ("O" == str) {
    return sf::Keyboard::Key::O;
  } else if ("P" == str) {
    return sf::Keyboard::Key::P;
  } else if ("Q" == str) {
    return sf::Keyboard::Key::Q;
  } else if ("R" == str) {
    return sf::Keyboard::Key::R;
  } else if ("S" == str) {
    return sf::Keyboard::Key::S;
  } else if ("T" == str) {
    return sf::Keyboard::Key::T;
  } else if ("U" == str) {
    return sf::Keyboard::Key::U;
  } else if ("V" == str) {
    return sf::Keyboard::Key::V;
  } else if ("W" == str) {
    return sf::Keyboard::Key::W;
  } else if ("X" == str) {
    return sf::Keyboard::Key::X;
  } else if ("Y" == str) {
    return sf::Keyboard::Key::Y;
  } else if ("Z" == str) {
    return sf::Keyboard::Key::Z;
  } else if ("Down" == str) {
    return sf::Keyboard::Key::Down;
  } else if ("Up" == str) {
    return sf::Keyboard::Key::Up;
  } else if ("Left" == str) {
    return sf::Keyboard::Key::Left;
  } else if ("Right" == str) {
    return sf::Keyboard::Key::Right;
  } else if ("Num2" == str) {
    return sf::Keyboard::Key::Num2;
  } else if ("Num8" == str) {
    return sf::Keyboard::Key::Num8;
  } else if ("Num4" == str) {
    return sf::Keyboard::Key::Num4;
  } else if ("Num6" == str) {
    return sf::Keyboard::Key::Num6;
  }

  std::cout << "unknown key " << str;
  return sf::Keyboard::Key::Unknown;
}

void KeyboardInputHandler::onKeyDown(const sf::Event::KeyEvent & event) {
  for (const auto& callback : _registeredInputs[event.code]) {
    callback();
  }
}

void KeyboardInputHandler::registerInput(const std::string & input, std::function<void()> callback) {
  _registeredInputs[stringToSfmlKey(input)].push_back(callback);
}

