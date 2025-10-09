#pragma once
#include "Constants.hpp"
#include "Enums.hpp"
#include "Hit.hpp"
#include "Tank.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <tuple>
#include <vector>

extern int g_score;
extern bool g_mute;

class Projectile {
private:
  int m_x{};
  int m_y{};
  Dir m_dir{}; // 0 - up, 1 - left, 2 - down, 3 - right
  int m_speed{};
  bool m_alive{true};
  std::shared_ptr<Tank> m_tank{};

public:
  Projectile(int x, int y, Dir dir, int speed, std::shared_ptr<Tank> tank)
      : m_x{x}, m_y{y}, m_dir{dir}, m_speed{speed}, m_tank{tank} {}
  ~Projectile() { m_tank->addShot(); }

  int getX() const { return m_x; }
  int getY() const { return m_y; }
  std::tuple<int, int> getSize() const {
    if (m_dir % 2)
      return {4, 3};
    else
      return {3, 4};
  }
  bool isAlive() const { return m_alive; }
  void kill() { m_alive = false; }
  bool isAP() const { return m_tank->getType() == general; }

  sf::Sprite *
  getSprite(const std::vector<std::unique_ptr<sf::Sprite>> &Sprites) const {
    return Sprites[m_dir].get();
  };

  void move() {
    switch (m_dir) {
    case up:
      m_y -= m_speed;
      break;
    case left:
      m_x -= m_speed;
      break;
    case down:
      m_y += m_speed;
      break;
    case right:
      m_x += m_speed;
      break;
    }
    if (m_x < 0 || m_y < 0 || m_x >= constants::maxX || m_y >= constants::maxY)
      m_alive = false;
  }

  bool checkBaseHit() {
    if ((m_dir == left && m_x < 112 && m_x > 95 && m_y > 189) ||
        (m_dir == down && m_x > 93 && m_x < 112 && m_y > 188) ||
        (m_dir == right && m_x > 92 && m_x < 109 && m_y > 191)) {
      m_alive = false;
      return true;
    }
    return false;
  }

  bool checkCollision(const std::vector<std::unique_ptr<Wall>> &objects) {
    for (const auto &obj : objects) {
      if (obj->isAlive()) {
        const int x{obj->getX()};
        const int y{obj->getY()};
        if ((!(m_dir % 2) && (x + 4 > m_x) && (x < m_x + 3) && (y + 4 > m_y) &&
             (y < m_y + 4)) ||
            ((m_dir % 2) && (y + 4 > m_y) && (y < m_y + 3) && (x + 4 > m_x) &&
             (x < m_x + 4))) {
          m_alive = false;
          return true;
        }
      }
    }
    return false;
  }

  bool checkCollision(const std::vector<std::unique_ptr<Projectile>> &objects) {
    for (const auto &obj : objects) {
      if (obj->isAlive() && obj.get() != this) {
        const int x{obj->getX()};
        const int y{obj->getY()};
        const auto [dx, dy]{obj->getSize()};
        if ((!(m_dir % 2) && (x + dx > m_x) && (x < m_x + 3) &&
             (y + dy > m_y) && (y < m_y + 4)) ||
            ((m_dir % 2) && (y + dy > m_y) && (y < m_y + 3) && (x + dx > m_x) &&
             (x < m_x + 4))) {
          m_alive = false;
          obj->kill();
          return true;
        }
      }
    }
    return false;
  }

  bool checkCollision(const std::vector<std::shared_ptr<Tank>> &objects,
                      std::vector<std::unique_ptr<Hit>> &Hits,
                      sf::Sound &bangSound1) {
    const TankType from{m_tank->getType()};
    for (const auto &obj : objects) {
      if (obj->isAlive()) {
        const auto [x, y, dx, dy]{obj->getSize()};
        const TankType to{obj->getType()};
        if ((!(m_dir % 2) && (x + dx > m_x) && (x < m_x + 3) &&
             (y + dy > m_y) && (y < m_y + 4)) ||
            ((m_dir % 2) && (y + dy > m_y) && (y < m_y + 3) && (x + dx > m_x) &&
             (x < m_x + 4))) {
          m_alive = false;
          if (!obj->isImmune()) {
            if (from <= general && to >= basic) {
              obj->hit();
              if (!obj->isAlive()) {
                Hits.emplace_back(std::make_unique<Hit>(x - 1, y - 1));
                g_score += 100 * (obj->getType() - 3);
                if (!g_mute)
                  bangSound1.play();
              }
            } else if (from >= basic && to <= general) {
              Hits.emplace_back(std::make_unique<Hit>(x - 1, y - 1));
              obj->hit();
              obj->respawn();
              if (!g_mute)
                bangSound1.play();
            } else if (from >= basic && to >= basic) {
              obj->disable(constants::friendlyFire);
            }
          }
          return true;
        }
      }
    }
    return false;
  }

  std::tuple<int, int> getHitPos() const {
    int x0{};
    int y0{};
    const int extraDepth{(m_tank->getType() == general) ? 4 : 0};
    switch (m_dir) {
    case up:
      x0 = (m_x + 1) / 8 * 8;
      y0 = m_y / 4 * 4 - extraDepth;
      return {(m_x - x0 > 3) ? x0 : x0 - 8, y0};
    case left:
      x0 = m_x / 4 * 4 - extraDepth;
      y0 = (m_y + 1) / 8 * 8;
      return {x0, (m_y - y0 > 3) ? y0 : y0 - 8};
    case down:
      x0 = (m_x + 1) / 8 * 8;
      y0 = (m_y + 3) / 4 * 4 + extraDepth;
      return {(m_x - x0 > 3) ? x0 : x0 - 8, y0 - 12};
    case right:
      x0 = (m_x + 3) / 4 * 4 + extraDepth;
      y0 = (m_y + 1) / 8 * 8;
      return {x0 - 12, (m_y - y0 > 3) ? y0 : y0 - 8};
    }
    return {0, 0};
  }
};

inline std::vector<std::unique_ptr<sf::Sprite>>
initProjectileSprites(const sf::Texture &Textures) {
  std::vector<std::unique_ptr<sf::Sprite>> objects{};
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({323, 102}, {3, 4})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({330, 102}, {4, 3})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({339, 102}, {3, 4})));
  objects.emplace_back(
      std::make_unique<sf::Sprite>(Textures, sf::IntRect({346, 102}, {4, 3})));
  return objects;
};
