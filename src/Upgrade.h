#pragma once
#include "Bang.h"
#include "Enums.h"
#include "Stages.h"
#include "Wall.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <vector>

extern const int g_refreshRate;
extern int g_stage;

class Upgrade {
private:
  UpgradeType m_type{};
  int m_x{};
  int m_y{};
  int m_alive{0};

public:
  Upgrade(UpgradeType type) : m_type(type) {};
  int getX() const { return m_x; }
  int getY() const { return m_y; }
  bool is_alive() const { return m_alive > 0; }
  void kill() { m_alive = 0; }
  void setAlive(int spot) {
    m_x = UpgradeStages[g_stage][spot][0];
    m_y = UpgradeStages[g_stage][spot][1];
    m_alive += 15 * g_refreshRate;
  }
  void tick() { --m_alive; }

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
