#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <vector>

extern const int g_updateRate;
extern const int g_maxX;
extern const int g_maxY;

class Projectile {
private:
  int m_x{};
  int m_y{};
  int m_dir{}; // 0 - up, 1 - left, 2 - down, 3 - right
  int m_speed{3};
  bool m_out{false};

public:
  Projectile(int x, int y, int dir) : m_x{x}, m_y{y}, m_dir{dir} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }
  bool is_out() const { return m_out; }

  void move() {
    switch (m_dir) {
    case 0:
      m_y -= m_speed;
      break;
    case 1:
      m_x -= m_speed;
      break;
    case 2:
      m_y += m_speed;
      break;
    case 3:
      m_x += m_speed;
      break;
    }
    if (m_x < 0 || m_y < 0 || m_x >= g_maxX || m_y >= g_maxY)
      m_out = true;
  }

  bool checkBaseHit() {
    if ((m_dir == 1 and m_x <= 111 and m_x >= 96 and m_y >= 190) ||
        (m_dir == 2 and m_x >= 94 and m_x <= 111 and m_y >= 189) ||
        (m_dir == 3 and m_x >= 93 and m_x <= 108 and m_y >= 190))
      return true;
    return false;
  }

  bool checkCollision(int x, int y) {
    if ((!(m_dir % 2) && (x + 4 > m_x) && (x < m_x + 3) && (y + 4 > m_y) &&
         (y < m_y + 4)) ||
        ((m_dir % 2) && (y + 4 > m_y) && (y < m_y + 3) && (x + 4 > m_x) &&
         (x < m_x + 4))) {
      return true;
    }
    return false;
  }

  std::shared_ptr<sf::Texture>
  getTexture(const std::vector<std::shared_ptr<sf::Texture>> &Textures) const {
    return Textures[m_dir];
  };

  std::tuple<int, int, int> getHitPos() const {
    int x0{};
    int y0{};
    switch (m_dir) {
    case 0:
      x0 = (m_x + 1) / 8 * 8;
      y0 = m_y / 4 * 4;
      return {(m_x - x0 > 3) ? x0 : x0 - 8, y0, m_dir};
    case 1:
      x0 = m_x / 4 * 4;
      y0 = (m_y + 1) / 8 * 8;
      return {x0, (m_y - y0 > 3) ? y0 : y0 - 8, m_dir};
    case 2:
      x0 = (m_x + 1) / 8 * 8;
      y0 = (m_y + 3) / 4 * 4 - 12;
      return {(m_x - x0 > 3) ? x0 : x0 - 8, y0, m_dir};
    case 3:
      x0 = (m_x + 3) / 4 * 4 - 12;
      y0 = (m_y + 1) / 8 * 8;
      return {x0, (m_y - y0 > 3) ? y0 : y0 - 8, m_dir};
    }
    return {0, 0, 0};
  }
};

inline std::vector<std::shared_ptr<sf::Texture>>
initProjectileTextures(const sf::Image &Sprites) {
  std::vector<std::shared_ptr<sf::Texture>> objects{};
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({323, 102}, {3, 4})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({330, 102}, {4, 3})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({339, 102}, {3, 4})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({346, 102}, {4, 3})));
  return objects;
};
