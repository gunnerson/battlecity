#pragma once
#include "Enums.h"
#include "Wall.h"
#include <SFML/Graphics/Texture.hpp>
#include <cstdlib>
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
  TankType m_type{}; // 0,1,2,3 - Player,
                     // 4 - Basic, 5 - Fast, 6 - Power, 7 - Armor
  int m_speed{};
  Color m_color{}; // 0 - yellow, 1 - gray, 2 - green, 3 - purple
  int m_health{};  // Hits left
  Dir m_dir{};     // 0 - up, 1 - left, 2 - down, 3 - right
  int m_x{};       // X offset from top left corner
  int m_y{};       // Y offset from top left corner
  int m_length{};  // Texture length
  int m_width{};   // Texture width
  int m_anim{0};   // Movement animation counter
  int m_reload{0}; // Reloading
  int m_shots{1};

public:
  Tank(TankType type, int x, int y,
       const std::vector<std::shared_ptr<sf::Texture>> &Textures)
      : m_type{type}, m_x{x}, m_y{y} {
    m_health = 1;
    switch (m_type) {
    case player:
      m_speed = 1;
      m_width = 13;
      m_length = 13;
      m_color = yellow;
      m_dir = up;
    case sergeant:
      m_speed = 1;
      m_width = 13;
      m_length = 16;
      m_color = yellow;
      m_dir = up;
    case colonel:
    case general:
      m_speed = 1;
      m_color = yellow;
      m_dir = up;
      break;
    case basic:
      m_speed = 1;
      m_color = gray;
      m_dir = down;
      break;
    case fast:
      m_speed = 3;
      m_color = gray;
      m_dir = down;
      m_shots = 2;
      break;
    case power:
      m_speed = 2;
      m_color = gray;
      m_dir = down;
      break;
    case armor:
      m_speed = 2;
      m_color = green;
      m_dir = down;
      m_health = 4;
      break;
    }
  }

  int getX() const { return m_x; }
  void setX(int x) { m_x = x; }
  int getY() const { return m_y; }
  void setY(int y) { m_y = y; }
  int getType() const { return m_type; }
  void setType(TankType type) { m_type = type; }
  int getDir() const { return m_dir; }
  void setDir(Dir dir) {
    if (std::abs(m_dir - dir) % 2) {
      switch (dir) {
      case up:
      case down:
        m_y = m_y / 16 * 16;
        break;
      case left:
      case right:
        m_x = m_x / 16 * 16;
        break;
      }
    }
    m_dir = dir;
  }

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
      m_color = gray;
  }

  std::shared_ptr<sf::Texture>
  getTexture(const std::vector<std::shared_ptr<sf::Texture>> &Textures) const {
    int idx{m_type * 32 + m_color * 8 + m_dir * 2};
    return m_anim % 2 ? Textures[idx] : Textures[++idx];
  };

  void updatePos(const std::vector<std::unique_ptr<Wall>> &Walls) {
    if (g_up)
      m_dir = up;
    else if (g_left)
      m_dir = left;
    else if (g_down)
      m_dir = down;
    else if (g_right)
      m_dir = right;

    bool collisionDetected = false;
    if (m_dir == up and m_y != 0) {
      for (const auto &obj : Walls) {
        if (obj->is_alive()) {
          int x{obj->getX()};
          int y{obj->getY()};
          if ((x + 4 > m_x) && (x < m_x + m_width) && (y + 4 == m_y)) {
            collisionDetected = true;
            break;
          }
        }
      }
      if (!collisionDetected) {
        m_y -= m_speed;
        ++m_anim;
      }

    } else if (m_dir == left and m_x != 0) {
      for (const auto &obj : Walls) {
        if (obj->is_alive()) {
          int x{obj->getX()};
          int y{obj->getY()};
          if ((y + 4 > m_y) && (y < m_y + m_width) && (x + 4 == m_x)) {
            collisionDetected = true;
            break;
          }
        }
      }
      if (!collisionDetected) {
        m_x -= m_speed;
        ++m_anim;
      }

    } else if (m_dir == down and m_y != g_maxY - m_length) {
      for (const auto &obj : Walls) {
        if (obj->is_alive()) {
          int x{obj->getX()};
          int y{obj->getY()};
          if ((x + 4 > m_x) && (x < m_x + m_width) && (y == m_y + m_length)) {
            collisionDetected = true;
            break;
          }
        }
      }
      if (!collisionDetected) {
        m_y += m_speed;
        ++m_anim;
      }

    } else if (m_dir == right and m_x != g_maxX - m_length) {
      for (const auto &obj : Walls) {
        if (obj->is_alive()) {
          int x{obj->getX()};
          int y{obj->getY()};
          if ((y + 4 > m_y) && (y < m_y + m_width) && (x == m_x + m_length)) {
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

  std::tuple<int, int, Dir, int> getProjectile() const {
    int speed{};
    switch (m_type) {
    case player:
      speed = 3;
      break;
    case sergeant:
    case colonel:
    case general:
      speed = 4;
      break;
    case basic:
      speed = 3;
      break;
    case fast:
      speed = 4;
      break;
    case power:
      speed = 5;
      break;
    case armor:
      speed = 4;
      break;
    }

    switch (m_dir) {
    case up:
      return {m_x + m_width / 2 - 1, m_y, m_dir, speed};
    case left:
      return {m_x, m_y + m_width / 2 - 1, m_dir, speed};
    case down:
      return {m_x + m_width / 2 - 1, m_y + m_length - 4, m_dir, speed};
    case right:
      return {m_x + m_length - 4, m_y + m_width / 2 - 1, m_dir, speed};
    }
    return {0, 0, up, 0};
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

          case player:
            dX = 13;
            dY = 13;
            switch (dir) {
            case up:
              ofX = 1;
              ofY = 2;
              break;
            case left:
              ofX = 2;
              ofY = 1;
              break;
            case down:
              ofX = 1;
              ofY = 1;
              break;
            case right:
              ofX = 1;
              ofY = 1;
              break;
            }
            break;

          case sergeant:
            switch (dir) {
            case up:
              dX = 13;
              dY = 16;
              ofX = 1;
              ofY = 0;
              break;
            case left:
              dX = 16;
              dY = 13;
              ofX = 0;
              ofY = 1;
              break;
            case down:
              dX = 13;
              dY = 16;
              ofX = 1;
              ofY = 0;
              break;
            case right:
              dX = 16;
              dY = 13;
              ofX = 0;
              ofY = 1;
              break;
            }
            break;

            // TODO: write offsets for all tank sprites
          }

          x = dir * 32 + color % 2 * 128 + anim * 16;
          y = type * 16 + color / 2 * 128;
          objects.emplace_back(std::make_shared<sf::Texture>(
              Sprites, false, sf::IntRect({x + ofX, y + ofY}, {dX, dY})));
        }
      }
    }
  }
  return objects;
};
