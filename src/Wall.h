#pragma once
#include "Stages.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
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

  std::tuple<int, int> getSize() const { return {4, 4}; }

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

inline std::vector<std::unique_ptr<Wall>> initWalls() {
  std::vector<std::unique_ptr<Wall>> objects{};
  for (std::size_t i{0}; i < WallStages[g_stage].size(); ++i) {
    const bool isBase{i < 8};
    switch (WallStages[g_stage][i][2]) {
    case 0:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 0, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 1, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1], 0, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1], 1, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 1, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 4,
                                                  WallStages[g_stage][i][1] + 4,
                                                  0, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 8,
                                                  WallStages[g_stage][i][1] + 4,
                                                  1, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1] + 4, 0, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 8, 0, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 4,
                                                  WallStages[g_stage][i][1] + 8,
                                                  1, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 8,
                                                  WallStages[g_stage][i][1] + 8,
                                                  0, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1] + 8, 1, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0],
                                 WallStages[g_stage][i][1] + 12, 1, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 4,
                                 WallStages[g_stage][i][1] + 12, 0, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 8,
                                 WallStages[g_stage][i][1] + 12, 1, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1] + 12, 0, isBase));
      break;

    case 2:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 0, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 1, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1], 0, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1], 1, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 1, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 4,
                                                  WallStages[g_stage][i][1] + 4,
                                                  0, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 8,
                                                  WallStages[g_stage][i][1] + 4,
                                                  1, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1] + 4, 0, isBase));
      break;

    case 3:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 0, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 1, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 1, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 4,
                                                  WallStages[g_stage][i][1] + 4,
                                                  0, isBase));
      break;

    case 6:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 2, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 3, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1], 2, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1], 3, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 4, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 4,
                                                  WallStages[g_stage][i][1] + 4,
                                                  5, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 8,
                                                  WallStages[g_stage][i][1] + 4,
                                                  4, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1] + 4, 5, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 8, 2, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 4,
                                                  WallStages[g_stage][i][1] + 8,
                                                  3, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 8,
                                                  WallStages[g_stage][i][1] + 8,
                                                  2, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1] + 8, 3, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0],
                                 WallStages[g_stage][i][1] + 12, 4, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 4,
                                 WallStages[g_stage][i][1] + 12, 5, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 8,
                                 WallStages[g_stage][i][1] + 12, 4, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1] + 12, 5, isBase));
      break;

    case 7:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 2, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 3, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 8, WallStages[g_stage][i][1], 2, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1], 3, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 4, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 4,
                                                  WallStages[g_stage][i][1] + 4,
                                                  5, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 8,
                                                  WallStages[g_stage][i][1] + 4,
                                                  4, isBase));
      objects.emplace_back(
          std::make_unique<Wall>(WallStages[g_stage][i][0] + 12,
                                 WallStages[g_stage][i][1] + 4, 5, isBase));
      break;

    case 8:
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1], 2, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0] + 4, WallStages[g_stage][i][1], 3, isBase));
      objects.emplace_back(std::make_unique<Wall>(
          WallStages[g_stage][i][0], WallStages[g_stage][i][1] + 4, 4, isBase));
      objects.emplace_back(std::make_unique<Wall>(WallStages[g_stage][i][0] + 4,
                                                  WallStages[g_stage][i][1] + 4,
                                                  5, isBase));
      break;

      // TODO: write remaining cases
    }
  }
  return objects;
}
