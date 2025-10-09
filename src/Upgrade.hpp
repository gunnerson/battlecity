#pragma once
#include "Constants.hpp"
#include "Enums.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <vector>

extern int g_stage;

class Upgrade {
private:
  UpgradeType m_type{};
  int m_x{};
  int m_y{};
  int m_alive{0};
  sf::Sprite *m_sprite{};

public:
  Upgrade(UpgradeType type,
          const std::vector<std::unique_ptr<sf::Sprite>> &Sprites)
      : m_type(type), m_sprite(Sprites[type].get()) {};

  int getX() const { return m_x; }
  int getY() const { return m_y; }
  sf::Sprite *getSprite() const { return m_sprite; }
  std::tuple<int, int> getSize() const { return {16, 16}; }
  bool isAlive() const { return m_alive > 0; }
  void kill() { m_alive = 0; }
  void tick() { --m_alive; }
  void setAlive(int x, int y) {
    m_x = x;
    m_y = y;
    m_sprite->setPosition({static_cast<float>(constants::ofX + m_x),
                           static_cast<float>(constants::ofY + m_y)});
    m_alive += constants::upgradeLifetime * constants::refreshRate;
  }

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
