#pragma once
#include "BrickWall.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

extern const int g_maxX;
extern const int g_maxY;
extern bool g_up;
extern bool g_left;
extern bool g_down;
extern bool g_right;

class Tank {
private:
  int m_type{};
  int m_speed{};
  int m_color{}; // 0 - yellow, 1 - white, 2 - green, 3 - purple
  int m_dir{};   // 0 - up, 1 - left, 2 - down, 3 - right
  int m_x{};     // X offset from top left corner
  int m_y{};     // Y offset from top left corner
  int m_anim{0}; // Movement animation counter
  sf::Texture m_texture1{};
  sf::Texture m_texture2{};

public:
  Tank(int type, int speed, int color, int dir, int x, int y,
       const sf::Image &Sprites)
      : m_type{type}, m_speed{speed}, m_color{color}, m_dir{dir}, m_x{x},
        m_y{y} {
    setTexture(Sprites);
  }

  int getDir() const { return m_dir; }
  void setDir(int dir) { m_dir = dir; }

  int getX() const { return m_x; }
  void setX(int x) { m_x = x; }

  int getY() const { return m_y; }
  void setY(int y) { m_y = y; }

  sf::Texture getTexture() const {
    return m_anim % 2 ? m_texture1 : m_texture2;
  };

  void setTexture(const sf::Image &Sprites) {
    int x = m_dir * 32 + m_color % 2 * 128;
    int y = m_type * 16 + m_color / 2 * 128;
    m_texture1 = sf::Texture(Sprites, false, sf::IntRect({x, y}, {16, 16}));
    m_texture2 =
        sf::Texture(Sprites, false, sf::IntRect({x + 16, y}, {16, 16}));
  };

  void updatePos(const std::vector<std::unique_ptr<BrickWall>> &BrickWalls) {
    if (g_up)
      m_dir = 0;
    else if (g_left)
      m_dir = 1;
    else if (g_down)
      m_dir = 2;
    else if (g_right)
      m_dir = 3;

    bool collisionDetected = false;
    // Going UP
    if (m_dir == 0 and m_y != 0) {
      for (const auto &obj : BrickWalls) {
        int x = obj->getX();
        int y = obj->getY();
        if ((x + 3 >= m_x) && (x <= m_x + 15) && (y + 4 == m_y)) {
          collisionDetected = true;
          break;
        };
      }
      if (!collisionDetected) {
        m_y -= m_speed;
        ++m_anim;
      }

      // Going Left
    } else if (m_dir == 1 and m_x != 0) {
      for (const auto &obj : BrickWalls) {
        int x = obj->getX();
        int y = obj->getY();
        if ((y + 3 >= m_y) && (y <= m_y + 15) && (x + 4 == m_x)) {
          collisionDetected = true;
          break;
        };
      }
      if (!collisionDetected) {
        m_x -= m_speed;
        ++m_anim;
      }

      // Going Down
    } else if (m_dir == 2 and m_y != g_maxY - 16) {
      for (const auto &obj : BrickWalls) {
        int x = obj->getX();
        int y = obj->getY();
        if ((x + 3 >= m_x) && (x <= m_x + 15) && (y == m_y + 16)) {
          collisionDetected = true;
          break;
        };
      }
      if (!collisionDetected) {
        m_y += m_speed;
        ++m_anim;
      }

      // Going Right
    } else if (m_dir == 3 and m_x != g_maxX - 16) {
      for (const auto &obj : BrickWalls) {
        int x = obj->getX();
        int y = obj->getY();
        if ((y + 4 > m_y) && (y < m_y + 16) && (x == m_x + 16)) {
          collisionDetected = true;
          break;
        };
      }
      if (!collisionDetected) {
        m_x += m_speed;
        ++m_anim;
      }
    }
  }
};
