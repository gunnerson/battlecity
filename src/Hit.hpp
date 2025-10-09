#pragma once
#include "Constants.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

class Hit {
private:
  int m_x{};
  int m_y{};
  int m_anim{0};
  bool m_ap{}; // Armor piercing

public:
  Hit(int x, int y, bool ap = false) : m_x{x}, m_y{y}, m_ap{ap} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }
  int getAnim() const { return m_anim; }
  int isAP() const { return m_ap; }

  bool isAlive() const { return m_anim < constants::refreshRate * 3 / 10 - 1; }
  void anim() { ++m_anim; }

  bool checkBlast(int x, int y) {
    if (((x == m_x) || (x == m_x + 4) || (x == m_x + 8) || (x == m_x + 12)) &&
        ((y == m_y) || (y == m_y + 4) || (y == m_y + 8) || (y == m_y + 12))) {
      return true;
    }
    return false;
  }

  sf::Sprite *
  getSprite(const std::vector<std::unique_ptr<sf::Sprite>> &Sprites) const {
    return Sprites[m_anim * 10 / constants::refreshRate].get();
  };
};

inline std::vector<std::unique_ptr<sf::Sprite>>
initHitSprites(const sf::Texture &Textures) {
  std::vector<std::unique_ptr<sf::Sprite>> objects{};
  objects.emplace_back(std::make_unique<sf::Sprite>(
      Textures, sf::IntRect({256, 128}, {16, 16})));
  objects.emplace_back(std::make_unique<sf::Sprite>(
      Textures, sf::IntRect({272, 128}, {16, 16})));
  objects.emplace_back(std::make_unique<sf::Sprite>(
      Textures, sf::IntRect({288, 128}, {16, 16})));
  return objects;
};
