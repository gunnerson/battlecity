#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <vector>

class Base {
private:
  int m_x{96};
  int m_y{192};
  bool m_alive{true};

public:
  int getX() const { return m_x; }
  int getY() const { return m_y; }
  bool isAlive() const { return m_alive; }
  void kill() { m_alive = false; }
  void setAlive() { m_alive = true; }

  sf::Sprite *
  getSprite(const std::vector<std::unique_ptr<sf::Sprite>> &Sprites) const {
    return Sprites[m_alive].get();
  };
};

inline std::vector<std::unique_ptr<sf::Sprite>>
initBaseSprites(const sf::Texture &Textures) {
  std::vector<std::unique_ptr<sf::Sprite>> objects{};
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({320, 32}, {16, 16})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({304, 32}, {16, 16})));
  return objects;
};
