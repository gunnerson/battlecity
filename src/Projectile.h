#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <utility>

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

  bool checkCollision(int x, int y) {
    if ((!(m_dir % 2) && (x + 4 > m_x) && (x < m_x + 3) && (y + 4 > m_y) &&
         (y < m_y + 4)) ||
        ((m_dir % 2) && (y + 4 > m_y) && (y < m_y + 3) && (x + 4 > m_x) &&
         (x < m_x + 4))) {
      return true;
    }
    return false;
  }

  sf::Texture
  getTexture(const std::vector<std::unique_ptr<sf::Texture>> &Textures) const {
    return *Textures[m_dir];
  };

  std::pair<int, int> getHitPos() const {
    switch (m_dir) {
    case 0:
      return {m_x + 1, m_y};
    case 1:
      return {m_x, m_y + 1};
    case 2:
      return {m_x + 1, m_y + 3};
    case 3:
      return {m_x + 3, m_y + 1};
    }
    return {0, 0};
  }
};

inline std::vector<std::unique_ptr<sf::Texture>>
initProjectileTextures(const sf::Image &Sprites) {
  std::vector<std::unique_ptr<sf::Texture>> objects{};
  objects.emplace_back(std::make_unique<sf::Texture>(
      Sprites, false, sf::IntRect({323, 102}, {3, 4})));
  objects.emplace_back(std::make_unique<sf::Texture>(
      Sprites, false, sf::IntRect({330, 102}, {4, 3})));
  objects.emplace_back(std::make_unique<sf::Texture>(
      Sprites, false, sf::IntRect({339, 102}, {3, 4})));
  objects.emplace_back(std::make_unique<sf::Texture>(
      Sprites, false, sf::IntRect({346, 102}, {4, 3})));
  return objects;
};
