#pragma once
#include "Constants.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

class Bang {
private:
  int m_x{};
  int m_y{};
  unsigned int m_anim{0};

public:
  Bang(int x, int y) : m_x{x}, m_y{y} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }
  int getAnim() const { return m_anim; }

  bool isAlive() const { return m_anim < constants::refreshRate * 3 / 10 - 1; }
  void anim() { ++m_anim; }

  sf::Sprite *
  getSprite(const std::vector<std::unique_ptr<sf::Sprite>> &Sprites) const {
    return Sprites[m_anim * 10 / constants::refreshRate].get();
  };
};

inline std::vector<std::unique_ptr<sf::Sprite>>
initBangSprites(const sf::Texture &Textures) {
  std::vector<std::unique_ptr<sf::Sprite>> objects{};
  objects.emplace_back(std::make_unique<sf::Sprite>(
      Textures, sf::IntRect({304, 128}, {32, 32})));
  objects.emplace_back(std::make_unique<sf::Sprite>(
      Textures, sf::IntRect({336, 128}, {32, 32})));
  objects.emplace_back(std::make_unique<sf::Sprite>(
      Textures, sf::IntRect({304, 128}, {32, 32})));
  return objects;
};
