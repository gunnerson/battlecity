//
// imports {{{1
#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <vector>

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
        {16, 16, 0},
        {48, 16, 0},
        {80, 16, 0},
        {112, 16, 0},
        {144, 16, 0},
        {176, 16, 0},

        {16, 32, 0},
        {48, 32, 0},
        {80, 32, 0},
        {112, 32, 0},
        {144, 32, 0},
        {176, 32, 0},

        {16, 48, 0},
        {48, 48, 0},
        {80, 48, 0},
        {112, 48, 0},
        {144, 48, 0},
        {176, 48, 0},

        {16, 64, 2},
        {48, 64, 2},
        {80, 64, 2},
        {112, 64, 2},
        {144, 64, 2},
        {176, 64, 2},

        {16, 72, 0},
        {48, 72, 0},
        {144, 72, 0},
        {176, 72, 0},

        {80, 88, 0},
        {112, 88, 0},

        {0, 104, 2},
        {32, 104, 0},
        {48, 104, 0},
        {32, 104, 0},
        {144, 104, 0},
        {160, 104, 0},
        {192, 104, 2},

        {80, 120, 2},
        {112, 120, 2},

        {80, 128, 0},
        {96, 128, 0},
        {112, 128, 0},

        {16, 136, 2},
        {48, 136, 2},
        {144, 136, 2},
        {176, 136, 2},

        {16, 144, 0},
        {48, 144, 0},
        {80, 144, 0},
        {112, 144, 0},
        {144, 144, 0},
        {176, 144, 0},

        {16, 160, 0},
        {48, 160, 0},
        {80, 160, 2},
        {112, 160, 2},
        {144, 160, 0},
        {176, 160, 0},

        {16, 176, 0},
        {48, 176, 0},
        {144, 176, 0},
        {176, 176, 0},

        // Base
        {88, 184, 2},
        {104, 184, 2},
        {88, 192, 3},
        {112, 192, 3},
        {88, 200, 3},
        {112, 200, 3},
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
    case 2:
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
    case 3:
      objects.emplace_back(std::make_unique<BrickWall>(
          brickWallsArray[g_stage][i][0], brickWallsArray[g_stage][i][1], 0));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 4,
                                      brickWallsArray[g_stage][i][1], 1));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0],
                                      brickWallsArray[g_stage][i][1] + 4, 1));
      objects.emplace_back(
          std::make_unique<BrickWall>(brickWallsArray[g_stage][i][0] + 4,
                                      brickWallsArray[g_stage][i][1] + 4, 0));
    }
  }
  return objects;
}
