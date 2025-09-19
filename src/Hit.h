#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

extern const int g_updateRate;

class Hit {
private:
  int m_x{};
  int m_y{};
  int m_dir{};
  int m_anim{0};

public:
  Hit(int x, int y, int dir) : m_x{x}, m_y{y}, m_dir{dir} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }
  int getAnim() const { return m_anim; }

  bool is_alive() const { return m_anim < 3; }
  void anim() { ++m_anim; }

  bool checkBlast(int x, int y) {
    int x0{m_x / 8 * 8};
    int y0{m_y / 8 * 8};
    int x1{(x - x0 > 3) ? x0 + 8 : x0 - 8};
    int y1{(y - y0 > 3) ? y0 + 8 : y0 - 8};
    if (((m_dir == 0) && (y + 3 == m_y) && (x >= m_x - 11) && (x <= m_x + 5)) ||
        ((m_dir == 1) && (x == m_x) && (y >= m_y - 11) && (y <= m_y + 5)) ||
        ((m_dir == 3) && (x == m_x) &&
         ((y == y0) || (y == y0 + 4) || (y == y1) || (y == y1 + 4)))) {
      return true;
    }
    return false;
  }

  sf::Texture
  getTexture(const std::vector<std::unique_ptr<sf::Texture>> &Textures) const {
    return *Textures[m_anim];
  };
};

inline std::vector<std::unique_ptr<sf::Texture>>
initHitTextures(const sf::Image &Sprites) {
  std::vector<std::unique_ptr<sf::Texture>> objects{};
  objects.emplace_back(std::make_unique<sf::Texture>(
      Sprites, false, sf::IntRect({256, 128}, {16, 16})));
  objects.emplace_back(std::make_unique<sf::Texture>(
      Sprites, false, sf::IntRect({272, 128}, {16, 16})));
  objects.emplace_back(std::make_unique<sf::Texture>(
      Sprites, false, sf::IntRect({288, 128}, {16, 16})));
  return objects;
};
