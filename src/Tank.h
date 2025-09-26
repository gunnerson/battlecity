#pragma once
#include "Wall.h"
#include <SFML/Graphics/Texture.hpp>
#include <vector>

extern const int g_maxX;
extern const int g_maxY;
extern const int g_updateRate;
extern bool g_up;
extern bool g_left;
extern bool g_down;
extern bool g_right;

const std::vector<std::vector<int>> NPCsArray = {
    // Stage 01
    {}

};

class Tank {
private:
  int m_type{}; // 0,1,2,3 - Player,
                // 4 - Basic, 5 - Fast, 6 - Power, 7 - Armor
  int m_speed{};
  int m_color{};   // 0 - yellow, 1 - white, 2 - green, 3 - purple
  int m_health{};  // Hits left
  int m_dir{};     // 0 - up, 1 - left, 2 - down, 3 - right
  int m_x{};       // X offset from top left corner
  int m_y{};       // Y offset from top left corner
  int m_dx{};      // Texture width
  int m_dy{};      // Texture height
  int m_anim{0};   // Movement animation counter
  int m_reload{0}; // Reloading
  int m_shots{1};

public:
  Tank(int type, int x, int y,
       const std::vector<std::shared_ptr<sf::Texture>> &Textures)
      : m_type{type}, m_x{x}, m_y{y} {
    auto texture{*getTexture(Textures)};
    auto textureSize{texture.getSize()};
    m_dx = textureSize.x;
    m_dy = textureSize.y;
    m_health = 1;
    switch (m_type) {
    case 0:
    case 1:
    case 2:
    case 3:
      m_speed = 1;
      m_color = 0;
      m_dir = 0;
      break;
    case 4:
      m_speed = 1;
      m_color = 1;
      m_dir = 2;
      break;
    case 5:
      m_speed = 3;
      m_color = 1;
      m_dir = 2;
      m_shots = 2;
      break;
    case 6:
      m_speed = 2;
      m_color = 1;
      m_dir = 2;
      break;
    case 7:
      m_speed = 2;
      m_color = 2;
      m_dir = 2;
      m_health = 4;
      break;
    }
  }

  int getX() const { return m_x; }
  void setX(int x) { m_x = x; }
  int getY() const { return m_y; }
  void setY(int y) { m_y = y; }
  int getType() const { return m_type; }
  void setType(int type) { m_type = type; }
  int getDir() const { return m_dir; }
  void setDir(int dir) { m_dir = dir; }

  bool is_alive() const { return m_health > 0; }

  void addShot() { ++m_shots; }
  void shoot() {
    --m_shots;
    m_reload = g_updateRate / 3;
  }
  bool canShoot() { return m_shots > 0 && !m_reload; }
  void reload() {
    if (m_reload)
      --m_reload;
  }

  void hit(bool fatal = false) {
    if (fatal)
      m_health = 0;
    else
      --m_health;
    if (m_type == 7 and m_health <= 1)
      m_color = 1;
  }

  std::shared_ptr<sf::Texture>
  getTexture(const std::vector<std::shared_ptr<sf::Texture>> &Textures) const {
    int idx{m_type * 32 + m_color * 8 + m_dir * 2};
    return m_anim % 2 ? Textures[idx] : Textures[++idx];
  };

  void updatePos(const std::vector<std::unique_ptr<Wall>> &Walls) {
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
      for (const auto &obj : Walls) {
        if (obj->is_alive()) {
          int x{obj->getX()};
          int y{obj->getY()};
          if ((x + 4 > m_x) && (x < m_x + m_dx) && (y + 4 == m_y)) {
            collisionDetected = true;
            break;
          }
        }
      }
      if (!collisionDetected) {
        m_y -= m_speed;
        ++m_anim;
      }

      // Going Left
    } else if (m_dir == 1 and m_x != 0) {
      for (const auto &obj : Walls) {
        if (obj->is_alive()) {
          int x{obj->getX()};
          int y{obj->getY()};
          if ((y + 4 > m_y) && (y < m_y + m_dy) && (x + 4 == m_x)) {
            collisionDetected = true;
            break;
          }
        }
      }
      if (!collisionDetected) {
        m_x -= m_speed;
        ++m_anim;
      }

      // Going Down
    } else if (m_dir == 2 and m_y != g_maxY - m_dy) {
      for (const auto &obj : Walls) {
        if (obj->is_alive()) {
          int x{obj->getX()};
          int y{obj->getY()};
          if ((x + 4 > m_x) && (x < m_x + m_dx) && (y == m_y + m_dy)) {
            collisionDetected = true;
            break;
          }
        }
      }
      if (!collisionDetected) {
        m_y += m_speed;
        ++m_anim;
      }

      // Going Right
    } else if (m_dir == 3 and m_x != g_maxX - m_dx) {
      for (const auto &obj : Walls) {
        if (obj->is_alive()) {
          int x{obj->getX()};
          int y{obj->getY()};
          if ((y + 4 > m_y) && (y < m_y + m_dy) && (x == m_x + m_dx)) {
            collisionDetected = true;
            break;
          }
        }
      }
      if (!collisionDetected) {
        m_x += m_speed;
        ++m_anim;
      }
    }
  }

  std::tuple<int, int, int, int> getProjectile() const {
    int speed{};
    switch (m_type) {
    case 0:
      speed = 3;
      break;
    case 1:
    case 2:
    case 3:
      speed = 4;
      break;
    case 4:
      speed = 3;
      break;
    case 5:
      speed = 4;
      break;
    case 6:
      speed = 5;
      break;
    case 7:
      speed = 4;
      break;
    }

    switch (m_dir) {
    case 0:
      return {m_x + m_dx / 2 - 1, m_y, m_dir, speed};
    case 1:
      return {m_x, m_y + m_dy / 2 - 1, m_dir, speed};
    case 2:
      return {m_x + m_dx / 2 - 1, m_y + m_dy - 4, m_dir, speed};
    case 3:
      return {m_x + m_dx - 4, m_y + m_dy / 2 - 1, m_dir, speed};
    }
    return {0, 0, 0, 0};
  }
};

inline std::vector<std::shared_ptr<sf::Texture>>
initTankTextures(const sf::Image &Sprites) {
  int x{};
  int y{};
  int dX{};
  int dY{};
  int ofX{};
  int ofY{};
  std::vector<std::shared_ptr<sf::Texture>> objects{};

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

          int x{dir * 32 + color % 2 * 128 + anim * 16};
          int y{type * 16 + color / 2 * 128};
          objects.emplace_back(std::make_shared<sf::Texture>(
              Sprites, false, sf::IntRect({x + ofX, y + ofY}, {dX, dY})));
        }
      }
    }
  }
  return objects;
};
