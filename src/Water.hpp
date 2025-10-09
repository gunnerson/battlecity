#pragma once
#include "Stages.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

extern int g_stage;

class Water {
private:
  int m_x{};
  int m_y{};

public:
  Water(int x, int y) : m_x{x}, m_y{y} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }
  bool isAlive() const { return true; }
  std::tuple<int, int, int, int> getSize() const { return {m_x, m_y, 16, 16}; }
};

inline void initWater(std::vector<std::unique_ptr<Water>> &objects) {
  for (std::size_t i{0}; i < WaterStages[g_stage].size(); ++i) {
    objects.emplace_back(
        std::make_unique<Water>(WaterStages[g_stage][i] % 100 * 16,
                                WaterStages[g_stage][i] / 100 * 16));
  }
}
