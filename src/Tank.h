#pragma once
#include "BrickWall.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

extern const int g_maxX;
extern const int g_maxY;
extern const int g_updateRate;
extern bool g_up;
extern bool g_left;
extern bool g_down;
extern bool g_right;

class Tank {
private:
  int m_type{};
  int m_speed{};
  int m_color{};   // 0 - yellow, 1 - white, 2 - green, 3 - purple
  int m_dir{};     // 0 - up, 1 - left, 2 - down, 3 - right
  int m_x{};       // X offset from top left corner
  int m_y{};       // Y offset from top left corner
  int m_dx{};      // Texture width
  int m_dy{};      // Texture height
  int m_anim{0};   // Movement animation counter
  int m_reload{0}; // Reloading
  sf::Texture m_texture1{};
  sf::Texture m_texture2{};

public:
  Tank(int type, int speed, int color, int dir, int x, int y,
       const std::vector<std::unique_ptr<sf::Texture>> &Textures)
      : m_type{type}, m_speed{speed}, m_color{color}, m_dir{dir}, m_x{x},
        m_y{y} {
    setTexture(Textures);
    auto textureSize = m_texture1.getSize();
    m_dx = textureSize.x;
    m_dy = textureSize.y;
  }

  int getDir() const { return m_dir; }
  void setDir(int dir) { m_dir = dir; }

  int getX() const { return m_x; }
  void setX(int x) { m_x = x; }
  int getY() const { return m_y; }
  void setY(int y) { m_y = y; }

  bool is_reloading() const { return m_reload; }
  void startReload() { m_reload = g_updateRate * 2; }
  void reload() {
    if (m_reload)
      --m_reload;
  }

  sf::Texture getTexture() const {
    return m_anim % 2 ? m_texture1 : m_texture2;
  };

  void setTexture(const std::vector<std::unique_ptr<sf::Texture>> &Textures) {
    int idx = m_type * 32 + m_color * 8 + m_dir * 2;
    m_texture1 = *Textures[idx];
    m_texture2 = *Textures[++idx];
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
        if ((x + 4 > m_x) && (x < m_x + m_dx) && (y + 4 == m_y)) {
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
        if ((y + 4 > m_y) && (y < m_y + m_dy) && (x + 4 == m_x)) {
          collisionDetected = true;
          break;
        };
      }
      if (!collisionDetected) {
        m_x -= m_speed;
        ++m_anim;
      }

      // Going Down
    } else if (m_dir == 2 and m_y != g_maxY - m_dy) {
      for (const auto &obj : BrickWalls) {
        int x = obj->getX();
        int y = obj->getY();
        if ((x + 4 > m_x) && (x < m_x + m_dx) && (y == m_y + m_dy)) {
          collisionDetected = true;
          break;
        };
      }
      if (!collisionDetected) {
        m_y += m_speed;
        ++m_anim;
      }

      // Going Right
    } else if (m_dir == 3 and m_x != g_maxX - m_dx) {
      for (const auto &obj : BrickWalls) {
        int x = obj->getX();
        int y = obj->getY();
        if ((y + 4 > m_y) && (y < m_y + m_dy) && (x == m_x + m_dx)) {
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

  std::tuple<int, int, int> getProjectilePos() const {
    switch (m_dir) {
    case 0:
      return {m_x + m_dx / 2 - 1, m_y - 1, m_dir};
    case 1:
      return {m_x - 1, m_y + m_dy / 2 - 1, m_dir};
    case 2:
      return {m_x + m_dx / 2 - 1, m_y + m_dy, m_dir};
    case 3:
      return {m_x + m_dx, m_y + m_dy / 2 - 1, m_dir};
    }
    return {0, 0, 0};
  }
};

inline std::vector<std::unique_ptr<sf::Texture>>
initTankTextures(const sf::Image &Sprites) {
  int x{};
  int y{};
  int dX{};
  int dY{};
  int ofX{};
  int ofY{};
  std::vector<std::unique_ptr<sf::Texture>> objects{};

  for (int type{0}; type < 8; ++type) {
    for (int color{0}; color < 4; ++color) {
      for (int dir{0}; dir < 4; ++dir) {
        for (int anim{0}; anim < 2; ++anim) {

          switch (type) {

          case 0:
            dX = 13;
            dY = 13;
            switch (dir) {
            case 0:
              ofX = 1;
              ofY = 2;
              break;
            case 1:
              ofX = 2;
              ofY = 1;
              break;
            case 2:
              ofX = 1;
              ofY = 1;
              break;
            case 3:
              ofX = 1;
              ofY = 1;
              break;
            }
            break;

            // TODO: write offsets for all tank sprites

          default:
            dX = 15;
            dY = 15;
            ofX = 1;
            ofY = 1;
          }

          int x = dir * 32 + color % 2 * 128 + anim * 16;
          int y = type * 16 + color / 2 * 128;
          auto obj = std::make_unique<sf::Texture>(
              Sprites, false, sf::IntRect({x + ofX, y + ofY}, {dX, dY}));
          objects.push_back(std::move(obj));
        }
      }
    }
  }
  return objects;
};
