#pragma once
#include "Stages.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

extern int g_stage;

class Wall {
private:
  int m_x{};
  int m_y{};
  int m_texture{};
  bool m_base{}; // Surrounds base
  bool m_alive{true};

public:
  Wall(int x, int y, int texture, bool base)
      : m_x{x}, m_y{y}, m_texture{texture}, m_base{base} {}

  int getX() const { return m_x; }
  void setX(int x) { m_x = x; }
  int getY() const { return m_y; }
  void setY(int y) { m_y = y; }
  bool isBase() const { return m_base; }

  std::tuple<int, int, int, int> getSize() const { return {m_x, m_y, 4, 4}; }

  bool isAlive() const { return m_alive; }
  void kill(bool ap = false) {
    if (m_texture < 2 || ap)
      m_alive = false;
  }

  sf::Sprite *
  getSprite(const std::vector<std::unique_ptr<sf::Sprite>> &Sprites) const {
    return Sprites[m_texture].get();
  };
};

inline std::vector<std::unique_ptr<sf::Sprite>>
initWallSprites(const sf::Texture &Textures) {
  std::vector<std::unique_ptr<sf::Sprite>> objects{};
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({256, 0}, {4, 4})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({260, 0}, {4, 4})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({256, 16}, {4, 4})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({260, 16}, {4, 4})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({256, 20}, {4, 4})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({260, 20}, {4, 4})));
  return objects;
}

inline void initWalls(std::vector<std::unique_ptr<Wall>> &objects,
                      int stage = g_stage, bool base_only = false) {
  for (std::size_t i{0}; i < (base_only ? 5 : WallStages[stage].size()); ++i) {
    int grid{WallStages[stage][i][0]};
    int texture{WallStages[stage][i][1]};
    int rotation{WallStages[stage][i][2]};
    for (int j{0}; j <= texture % 4; ++j) {
      for (int k{0}; k < 4; ++k) {
        objects.emplace_back(std::make_unique<Wall>(
            grid % 100 * 16 + rotation % 2 * 8 + j % 2 * 8 -
                8 * (j && rotation == 1) -
                ((texture == 1 || texture == 5) && rotation == 3) * 8 -
                (j && rotation == 3) * 8 + k % 2 * 4,
            grid / 100 * 16 + rotation / 2 * 8 + (j && rotation % 2) * 8 +
                j / 2 * 8 -
                ((texture == 1 || texture == 5) && rotation == 3) * 8 +
                k / 2 * 4,
            (texture / 4) ? k + 2 : (k % 2 + k / 2) % 2, i < 6));
      }
    }
  }
}
