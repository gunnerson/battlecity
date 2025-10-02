#pragma once
#include "Enums.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <vector>

class Upgrade {
private:
  int m_x{};
  int m_y{};
  UpgradeType m_type{};
  bool m_alive{true};

public:
  Upgrade(UpgradeType type) : m_type(type) {};
  int getX() const { return m_x; }
  int getY() const { return m_y; }
  bool is_alive() const { return m_alive; }
  void kill() { m_alive = false; }

  sf::Sprite *
  getSprite(const std::vector<std::unique_ptr<sf::Sprite>> &Sprites) const {
    return Sprites[m_type].get();
  };
};

inline std::vector<std::unique_ptr<sf::Sprite>>
initUpgradeSprites(const sf::Texture &Textures) {
  std::vector<std::unique_ptr<sf::Sprite>> objects{};
  for (int i{0}; i < 6; ++i) {
    objects.emplace_back(std::make_unique<sf::Sprite>(
        Textures, sf::IntRect({256 + 16 * i, 112}, {16, 16})));
  }
  return objects;
};
