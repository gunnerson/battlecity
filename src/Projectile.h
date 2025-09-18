#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <utility>

extern const int g_updateRate;

class Projectile {
private:
  int m_x{};
  int m_y{};
  int m_dir{}; // 0 - up, 1 - left, 2 - down, 3 - right
  int m_speed{3};

public:
  Projectile(int x, int y, int dir) : m_x{x}, m_y{y}, m_dir{dir} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }

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
  }

  bool checkCollision(int x, int y) {
    if (((m_dir == 0) && (x + 4 > m_x) && (x < m_x + 3) && (y + 3 == m_y)) ||
        ((m_dir == 1) && (y + 4 > m_y) && (y < m_y + 3) && (x + 3 == m_x)) ||
        ((m_dir == 2) && (x + 4 > m_x) && (x < m_x + 3) && (y == m_y + 3)) ||
        ((m_dir == 3) && (y + 4 > m_y) && (y < m_y + 3) && (x == m_x + 3))) {
      return true;
    }
    return false;
  }

  sf::Texture
  getTexture(const std::vector<std::unique_ptr<sf::Texture>> &Textures) const {
    return *Textures[m_dir];
  };
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
