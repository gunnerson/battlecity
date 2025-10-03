#pragma once
#include "Stages.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

extern int g_stage;

class Wall {
private:
  int m_x{};
  int m_y{};
  int m_texture{};
  bool m_alive{true};

public:
  Wall(int x, int y, int texture) : m_x{x}, m_y{y}, m_texture{texture} {}

  int getX() const { return m_x; }
  void setX(int x) { m_x = x; }

  int getY() const { return m_y; }
  void setY(int y) { m_y = y; }

  std::tuple<int, int> getSize() const { return {4, 4}; }

  bool isAlive() const { return m_alive; }
  void kill() {
    if (m_texture < 2)
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

inline std::vector<std::unique_ptr<Wall>> initWalls() {
  std::vector<std::unique_ptr<Wall>> objects{};
  for (std::size_t i{0}; i < WallStages[g_stage].size(); ++i) {
    switch (WallStages[g_stage][i][2]) {
    case 0:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 0));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1], 0));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1], 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1] + 4, 0));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1] + 4, 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1] + 4, 0));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 8, 0));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1] + 8, 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1] + 8, 0));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1] + 8, 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 12, 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1] + 12, 0));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1] + 12, 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1] + 12, 0));
      break;

    case 2:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 0));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1], 0));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1], 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1] + 4, 0));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1] + 4, 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1] + 4, 0));
      break;

    case 3:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 0));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 1));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1] + 4, 0));
      break;

    case 6:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 2));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 3));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1], 2));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1], 3));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 4));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1] + 4, 5));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1] + 4, 4));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1] + 4, 5));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 8, 2));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1] + 8, 3));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1] + 8, 2));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1] + 8, 3));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 12, 4));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1] + 12, 5));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1] + 12, 4));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1] + 12, 5));
      break;

    case 7:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 2));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 3));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1], 2));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1], 3));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 4));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1] + 4, 5));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1] + 4, 4));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 12, WallStages[g_stage][i][1] + 4, 5));
      break;

    case 8:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 2));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 3));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 4));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1] + 4, 5));
      break;

      // TODO: write remaining cases
    }
  }
  return objects;
}
