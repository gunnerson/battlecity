#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>

extern const int g_maxX;
extern const int g_maxY;

class Tank {
private:
  int m_type{};
  int m_speed{};
  int m_color{}; // 0 - yellow, 1 - white, 2 - green, 3 - purple
  int m_dir{};   // 0 - up, 1 - left, 2 - down, 3 - right
  int m_x{};     // X offset from top left corner
  int m_y{};     // Y offset from top left corner
  int m_anim{};

public:
  Tank(int type, int speed, int color, int dir, int x, int y) {
    m_type = type;
    m_speed = speed;
    m_color = color;
    m_dir = dir;
    m_x = x;
    m_y = y;
    m_anim = 0;
  }

  int getDir() const { return m_dir; }
  void setDir(int dir) { m_dir = dir; }

  int getX() const { return m_x; }
  void setX(int x) { m_x = x; }

  int getY() const { return m_y; }
  void setY(int y) { m_y = y; }

  sf::Texture getTexture(sf::Image Sprites) const {
    int x = m_dir * 32 + m_color % 2 * 128 + m_anim % 2 * 16;
    int y = m_type * 16 + m_color / 2 * 128;
    sf::Texture texture(Sprites, false, sf::IntRect({x, y}, {16, 16}));
    return texture;
  };

  void updatePos() {
    if (m_dir == 0 and m_y != 0) {
      m_y -= m_speed;
      ++m_anim;
    } else if (m_dir == 1 and m_x != 0) {
      m_x -= m_speed;
      ++m_anim;
    } else if (m_dir == 2 and m_y != g_maxY - 16) {
      m_y += m_speed;
      ++m_anim;
    } else if (m_dir == 3 and m_x != g_maxX - 16) {
      m_x += m_speed;
      ++m_anim;
    }
  }
};
