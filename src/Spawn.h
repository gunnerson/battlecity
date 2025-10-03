#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <vector>

class Spawn {
private:
  int m_x{};
  int m_y{};
  int m_anim{0};

public:
  Spawn(int x, int y) : m_x{x}, m_y{y} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }

  bool isAlive() const { return m_anim < 4; }
  void anim() { ++m_anim; }

  sf::Sprite *
  getSprite(const std::vector<std::unique_ptr<sf::Sprite>> &Sprites) const {
    return Sprites[m_anim].get();
  };
};

inline std::vector<std::unique_ptr<sf::Sprite>>
initSpawnSprites(const sf::Texture &Textures) {
  std::vector<std::unique_ptr<sf::Sprite>> objects{};
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({256, 96}, {15, 15})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({272, 96}, {15, 15})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({288, 96}, {15, 15})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({304, 96}, {15, 15})));
  return objects;
};
