//
// imports {{{1
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

extern int g_stage;

// Map {{{1
// {Pos X, Pos Y, Texture N}
// 0:   0101
//      1010
//      0101
//      1010
//
// 1:   01    01
//      10    10
//      01010101
//      10101010
//
// 2:   0101
//      1010
//
// 3:   01
//      10
//
// 4:   1  0
//      0101
//
// 5:   01
const std::vector<std::vector<std::vector<int>>> brickWallsArray = {
    // Stage 01
    {
        // at 5, 11
        {80, 176, 0},
        // at 6, 11
        {96, 176, 0},
        // at 7, 11
        {112, 176, 0},
        // at 5, 12
        {80, 192, 0},
        // at 7, 12
        {112, 192, 0},
    }};
// }}}1

class BrickWall {
private:
  int m_x{};
  int m_y{};
  int m_texture{};
  bool m_alive{true};

public:
  BrickWall(int x, int y, int texture) : m_x{x}, m_y{y}, m_texture{texture} {}

  int getX() const { return m_x; }
  void setX(int x) { m_x = x; }

  int getY() const { return m_y; }
  void setY(int y) { m_y = y; }

  bool is_alive() const { return m_alive; }
  void kill() { m_alive = false; }

  std::shared_ptr<sf::Texture>
  getTexture(const std::vector<std::shared_ptr<sf::Texture>> &Textures) const {
    return Textures[m_texture];
  };
};

inline std::vector<std::shared_ptr<sf::Texture>>
initBrickWallTextures(const sf::Image &Sprites) {
  std::vector<std::shared_ptr<sf::Texture>> objects{};
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({256, 0}, {4, 4})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({260, 0}, {4, 4})));
  return objects;
}

inline std::vector<std::unique_ptr<BrickWall>> initBrickWalls() {
  std::vector<std::unique_ptr<BrickWall>> objects{};
  for (std::size_t i{0}; i < brickWallsArray[g_stage].size(); ++i) {
    switch (brickWallsArray[g_stage][i][2]) {
    case 0:
      objects.emplace_back(std::make_unique<BrickWall>(
          brickWallsArray[g_stage][i][0], brickWallsArray[g_stage][i][1], 0));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 4,
                                      brickWallsArray[g_stage][i][1], 1));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 8,
                                      brickWallsArray[g_stage][i][1], 0));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 12,
                                      brickWallsArray[g_stage][i][1], 1));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0],
                                      brickWallsArray[g_stage][i][1] + 4, 1));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 4,
                                      brickWallsArray[g_stage][i][1] + 4, 0));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 8,
                                      brickWallsArray[g_stage][i][1] + 4, 1));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 12,
                                      brickWallsArray[g_stage][i][1] + 4, 0));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0],
                                      brickWallsArray[g_stage][i][1] + 8, 0));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 4,
                                      brickWallsArray[g_stage][i][1] + 8, 1));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 8,
                                      brickWallsArray[g_stage][i][1] + 8, 0));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 12,
                                      brickWallsArray[g_stage][i][1] + 8, 1));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0],
                                      brickWallsArray[g_stage][i][1] + 12, 1));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 4,
                                      brickWallsArray[g_stage][i][1] + 12, 0));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 8,
                                      brickWallsArray[g_stage][i][1] + 12, 1));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 12,
                                      brickWallsArray[g_stage][i][1] + 12, 0));
    }
  }
  return objects;
}
