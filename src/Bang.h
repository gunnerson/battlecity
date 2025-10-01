#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>

extern const int g_refreshRate;

class Bang {
private:
  int m_x{};
  int m_y{};
  int m_anim{0};

public:
  Bang(int x, int y) : m_x{x}, m_y{y} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }
  int getAnim() const { return m_anim; }

  bool is_alive() const { return m_anim < g_refreshRate * 3 / 5 - 1; }
  void anim() { ++m_anim; }

  sf::Sprite *
  getSprite(const std::vector<std::unique_ptr<sf::Sprite>> &Sprites) const {
    return Sprites[m_anim * 5 / g_refreshRate].get();
  };
};

inline std::vector<std::unique_ptr<sf::Sprite>>
initBangSprites(const sf::Texture &Textures) {
  std::vector<std::unique_ptr<sf::Sprite>> objects{};
  objects.emplace_back(std::make_unique<sf::Sprite>(
      Textures, sf::IntRect({304, 128}, {32, 32})));
  objects.emplace_back(std::make_unique<sf::Sprite>(
      Textures, sf::IntRect({336, 128}, {32, 32})));
  objects.emplace_back(std::make_unique<sf::Sprite>(
      Textures, sf::IntRect({304, 128}, {32, 32})));
  return objects;
};
