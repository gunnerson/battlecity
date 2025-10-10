// imports and globals {{{1
#pragma once
#include "Bang.hpp"
#include "Constants.hpp"
#include "Enums.hpp"
#include "Ice.hpp"
#include "Upgrade.hpp"
#include "Wall.hpp"
#include "Water.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <vector>

extern bool g_mute;
extern int g_score;
extern int g_shovel;
extern bool g_up;
extern bool g_left;
extern bool g_down;
extern bool g_right;
// }}}1

class Tank {
private:
  TankType m_type{}; // 0,1,2,3 - Player,
                     // 4 - Basic, 5 - Fast, 6 - Power, 7 - Armor
  int m_x{};         // X offset from top left corner
  int m_y{};         // Y offset from top left corner
  int m_speed{};     // Pixels per 1/refreshRate
  Color m_color{};   // 0 - yellow, 1 - gray, 2 - green, 3 - red
  Dir m_dir{};       // 0 - up, 1 - left, 2 - down, 3 - right
  std::vector<int> m_slide{0, 0, 0, 0}; // Slide left
  bool m_upgrade{};                     // Drops upgrade when shot
  int m_length{};                       // Texture length
  int m_width{};                        // Texture width
  unsigned int m_anim{0};               // Movement animation counter
  unsigned int m_flash{0};              // Flashing animation counter
  int m_health{1};                      // Hits left
  int m_reload{0};                      // Reloading
  int m_fire{1};                        // Fire rate
  int m_shots{1};                       // Shots left
  int m_nextTurn{constants::refreshRate *
                 constants::npcTurnFrequency}; // Time to next turn
  int m_disable{constants::refreshRate};       // Prevents from moving
  int m_immunity{constants::refreshRate * 2};  // Immune to hits
  std::vector<int> m_dirProb{0, 0, 0, 0}; // Array of probabilities for turning
  std::vector<int> m_dirRestrict{0, 1, 1, 1}; // Array of turn restrictions

public:
  // Constructor {{{1
  Tank(TankType type, int x, int y, bool upgrade = false)
      : m_type{type}, m_x{x}, m_y{y}, m_upgrade(upgrade) {
    switch (m_type) {
    case player:
    case sergeant:
    case colonel:
    case general:
      m_speed = 2;
      m_width = 13;
      m_length = 13;
      m_color = yellow;
      m_dir = up;
      m_x += 1;
      m_y += 3;
      m_health = 3;
      break;
    case basic:
      m_speed = 1;
      m_width = 13;
      m_length = 15;
      m_color = gray;
      m_dir = down;
      m_x += 1;
      break;
    case fast:
      m_speed = 3;
      m_width = 13;
      m_length = 15;
      m_color = gray;
      m_dir = down;
      m_x += 1;
      break;
    case power:
      m_speed = 2;
      m_width = 13;
      m_length = 15;
      m_color = gray;
      m_dir = down;
      m_x += 1;
      m_fire = 2;
      m_shots = 2;
      break;
    case armor:
      m_speed = 1;
      m_width = 13;
      m_length = 15;
      m_color = green;
      m_dir = down;
      m_x += 1;
      m_health = 4;
      break;
    }
  }
  // }}}1

  int getX() const { return m_x; }
  void setX(int x) { m_x = x; }
  int getY() const { return m_y; }
  void setY(int y) { m_y = y; }
  Dir getDir() const { return m_dir; }
  TankType getType() const { return m_type; }
  bool isUpgrade() const { return m_upgrade; }
  void flash() { ++m_flash; }

  int getHealth() const { return m_health; }
  bool isAlive() const { return m_health > 0; }
  void addLife() { ++m_health; }
  void reset() {
    m_health = 3;
    m_type = player;
    m_length = 13;
    m_width = 13;
    m_reload = 0;
    m_fire = 1;
    m_shots = 1;
  }

  int getImmunity() { return (m_immunity > 0) ? --m_immunity : m_immunity; }
  bool isImmune() const { return m_immunity > 0; }
  void setImmunity(int time) { m_immunity = time * constants::refreshRate; }
  void addSlide() { m_slide[m_dir] = constants::slide; }

  std::tuple<int, int, int, int> getSize() const {
    if (m_dir % 2)
      return {m_x, m_y, m_length, m_width};
    else
      return {m_x, m_y, m_width, m_length};
    ;
  }

  void addShot() { ++m_shots; }
  void shoot() {
    --m_shots;
    m_reload = constants::refreshRate / constants::npcShootFrequency;
  }
  bool canShoot() { return m_shots > 0 && m_reload <= 0 && m_disable <= 0; }
  void reload() {
    if (m_reload > 0)
      m_reload -= m_fire;
  }

  void hit(bool fatal = false) {
    if (m_type > player && m_type <= general)
      upgrade(true);
    if (fatal)
      m_health = 0;
    else
      --m_health;
    if (m_type == armor && m_health <= 1)
      m_color = gray;
  }

  void disable(int time) { m_disable += constants::refreshRate * time; }
  bool isDisabled() const { return m_disable > 0; }
  void repair() { --m_disable; }

  void respawn() {
    m_x = 64;
    m_y = 208 - m_length;
    m_dir = up;
    m_disable = constants::refreshRate;
    m_immunity = constants::refreshRate * 2;
  }

  sf::Sprite *
  getSprite(const std::vector<std::unique_ptr<sf::Sprite>> &Sprites) const {
    int idx{m_type * 32 + ((m_upgrade && m_flash % 4 > 1) ? 24 : m_color * 8) +
            m_dir * 2};
    if (m_color == green)
      idx -= 8 * (m_flash % 4 > 1);
    return m_anim % 2 ? Sprites[idx].get() : Sprites[++idx].get();
  };

  // upgrade {{{1
  void upgrade(bool reset = false) {
    const int extraShots{(m_type > sergeant) ? 1 : 0};
    if (m_type < general) {
      m_type =
          reset ? player : static_cast<TankType>(static_cast<int>(m_type) + 1);
      switch (m_type) {
      case player:
        m_length = 13;
        m_width = 13;
        m_shots -= extraShots;
        break;
      case sergeant:
        m_length = 16;
        switch (m_dir) {
        case up:
        case left:
          break;
        case down:
          m_y -= 3;
          break;
        case right:
          m_x -= 3;
        }
        break;
      case colonel:
        ++m_shots;
        m_length = 15;
        break;
      case general:
        m_length = 15;
        m_width = 14;
        switch (m_dir) {
        case up:
        case down:
          if (m_x % 4 == 3)
            --m_x;
          break;
        case left:
        case right:
          if (m_y % 4 == 3)
            --m_y;
          break;
        }
      default:
        break;
      }
    }
  }

  // setDir {{{1
  void setDir(Dir dir) {
    if (std::abs(m_dir - dir) % 2) {
      switch (dir) {
      case up:
        m_y = std::max(m_y / 4 * 4, m_y - m_length + m_width);
        m_x = (m_dir == left) ? m_x : m_x + m_length - m_width;
        break;
      case down:
        m_y = std::min(m_y / 4 * 4 + 16 - m_length, m_y);
        m_x = (m_dir == left) ? m_x : m_x + m_length - m_width;
        break;
      case left:
        m_x = std::max(m_x / 4 * 4, m_x - m_length + m_width);
        m_y = (m_dir == up) ? m_y : m_y + m_length - m_width;
        break;
      case right:
        m_x = std::min(m_x / 4 * 4 + 16 - m_length, m_x);
        m_y = (m_dir == up) ? m_y : m_y + m_length - m_width;
        break;
      }
    }
    m_dir = dir;
  }

  // checkOutOfBounds {{{1
  bool checkOutOfBounds() {
    bool outOfScreen{false};
    if (m_dir == up) {
      // Check out of screen
      if (m_y - m_speed < 0) {
        if (m_y)
          ++m_anim;
        m_y = 0;
        m_dirRestrict[up] = 0;
        outOfScreen = true;
      }
    } else if (m_dir == left) {
      // Check out of screen
      if (m_x - m_speed < 0) {
        if (m_x)
          ++m_anim;
        m_x = 0;
        m_dirRestrict[left] = 0;
        outOfScreen = true;
        // Check base
      } else if (m_y + m_width > 192 && m_x - m_speed < 112 &&
                 m_x - m_speed > 95) {
        if (m_x != 112)
          ++m_anim;
        m_x = 112;
        outOfScreen = true;
      }
    } else if (m_dir == down) {
      // Check out of screen
      if (m_y + m_length + m_speed >= constants::maxY) {
        if (m_y + m_length < constants::maxY)
          ++m_anim;
        m_y = constants::maxY - m_length;
        m_dirRestrict[down] = 0;
        outOfScreen = true;
        // Check base
      } else if (m_x + m_width > 96 && m_x < 112 &&
                 m_y + m_length + m_speed > 191) {
        if (m_y != 192 - m_length)
          ++m_anim;
        m_y = 192 - m_length;
        outOfScreen = true;
      }
    } else if (m_dir == right) {
      // Check out of screen
      if (m_x + m_length + m_speed >= constants::maxX) {
        if (m_x + m_length < constants::maxX)
          ++m_anim;
        m_x = constants::maxX - m_length;
        m_dirRestrict[right] = 0;
        outOfScreen = true;
        // Check base
      } else if (m_y + m_width > 192 && m_x + m_length + m_speed > 95 &&
                 m_x + m_length + m_speed < 111) {
        if (m_x != 96 - m_length)
          ++m_anim;
        m_x = 96 - m_length;
        outOfScreen = true;
      }
    }
    return outOfScreen;
  }

  // checkUpgrades {{{1
  void checkUpgrades(const std::vector<std::shared_ptr<Tank>> &Tanks,
                     std::vector<std::unique_ptr<Bang>> &Bangs,
                     std::vector<std::unique_ptr<Wall>> &Walls,
                     const std::vector<std::unique_ptr<Upgrade>> &Upgrades,
                     sf::Sound &upgradePickSound, sf::Sound &bangSound) {
    for (std::size_t i{0}; i < Upgrades.size(); ++i) {
      if (Upgrades[i]->isAlive()) {
        const int x{Upgrades[i]->getX()};
        const int y{Upgrades[i]->getY()};
        const auto [x0, y0, dx, dy]{getSize()};
        if ((m_x < x + 16) && (x < m_x + dx) && (m_y < y + 16) &&
            (y < m_y + dy)) {
          int c{0};
          switch (i) {

            // helmet
          case 0:
            setImmunity(constants::upgradeHelmet);
            break;

            // freeze
          case 1:
            for (const auto &tank : Tanks) {
              if (tank->getType() >= basic)
                tank->disable(constants::upgradeFreeze);
            }
            break;

            // shovel
          case 2:
            g_shovel = constants::upgradeShovel * constants::refreshRate;
            Walls.erase(std::remove_if(Walls.begin(), Walls.end(),
                                       [](const std::unique_ptr<Wall> &obj) {
                                         return obj->isBase();
                                       }),
                        Walls.end());
            initWalls(Walls, 0, true);
            for (const auto &tank : Tanks) {
              const auto [x, y, dx, dy]{tank->getSize()};
              if (m_y + dy > 183 && m_x + dx > 87 && m_x < 120) {
                tank->hit(true);
                Bangs.emplace_back(
                    std::make_unique<Bang>(x + dx / 2 - 16, y + dy / 2 - 16));
                if (!g_mute)
                  bangSound.play();
              }
            }
            break;

            // star
          case 3:
            upgrade();
            break;

            // grenade
          case 4:
            for (const auto &tank : Tanks) {
              if (tank->getType() >= basic) {
                tank->hit(true);
                const auto [x, y, dx, dy]{tank->getSize()};
                Bangs.emplace_back(
                    std::make_unique<Bang>(x + dx / 2 - 16, y + dy / 2 - 16));
                if (!g_mute)
                  bangSound.play();
              }
            }
            break;

            // life
          case 5:
            addLife();
            break;
          }
          Upgrades[i]->kill();
          g_score += 500;
          if (!g_mute)
            upgradePickSound.play();
        }
      }
    }
  }

  // checkCollision {{{1
  template <typename T> bool checkCollision(const std::vector<T> &objects) {
    bool collisionDetected{false};
    if (m_dir == up) {
      for (const auto &obj : objects) {
        if (obj->isAlive()) {
          const auto [x, y, dx, dy]{obj->getSize()};
          if ((x + dx > m_x) && (x < m_x + m_width) &&
              (y + dy > m_y - m_speed) && (y < m_y - m_speed)) {
            collisionDetected = true;
            if (m_y != y + dy)
              ++m_anim;
            m_y = y + dy;
            m_dirRestrict[up] = 0;
            break;
          }
        }
      }

    } else if (m_dir == left) {
      for (const auto &obj : objects) {
        if (obj->isAlive()) {
          const auto [x, y, dx, dy]{obj->getSize()};
          if ((y + dy > m_y) && (y < m_y + m_width) &&
              (x + dx > m_x - m_speed) && (x < m_x - m_speed)) {
            collisionDetected = true;
            if (m_x != x + dx)
              ++m_anim;
            m_x = x + dx;
            m_dirRestrict[left] = 0;
            break;
          }
        }
      }

    } else if (m_dir == down) {
      for (const auto &obj : objects) {
        if (obj->isAlive()) {
          const auto [x, y, dx, dy]{obj->getSize()};
          if ((x + dx > m_x) && (x < m_x + m_width) &&
              (y < m_y + m_length + m_speed) &&
              (y + dy > m_y + m_length + m_speed)) {
            collisionDetected = true;
            m_y = y - m_length;
            m_dirRestrict[down] = 0;
            break;
          }
        }
      }

    } else if (m_dir == right) {
      for (const auto &obj : objects) {
        if (obj->isAlive()) {
          const auto [x, y, dx, dy]{obj->getSize()};
          if ((y + dy > m_y) && (y < m_y + m_width) &&
              (x < m_x + m_length + m_speed) &&
              (x + dx > m_x + m_length + m_speed)) {
            collisionDetected = true;
            m_x = x - m_length;
            m_dirRestrict[right] = 0;
            break;
          }
        }
      }
    }
    return collisionDetected;
  }

  // checkIce {{{1
  void checkIce(const std::vector<std::unique_ptr<Ice>> &objects) {
    const auto [x0, y0, dx0, dy0]{getSize()};
    const bool isMoving{g_up || g_left || g_down || g_right};
    bool onIce{false};
    for (const auto &obj : objects) {
      const auto [x, y, dx, dy]{obj->getSize()};
      if ((m_x + dx0 > x) && (m_x < x + dx) && (m_y + dy0 > y) &&
          (m_y < y + dy)) {
        onIce = true;
        if (isMoving)
          m_slide[m_dir] = constants::slide;
        break;
      }
    }
    if (onIce) {
      for (int i{0}; i < 4; ++i) {
        if ((m_dir != i || !isMoving) && m_slide[i])
          --m_slide[i];
      }
    } else {
      m_slide = {0, 0, 0, 0};
    }
    if (m_type <= general || m_type == power)
      m_speed = onIce ? 1 : 2;
    else if (m_type == fast)
      m_speed = onIce ? 2 : 3;
  }

  // updatePos {{{1
  void updatePos(const std::vector<std::unique_ptr<Wall>> &Walls,
                 const std::vector<std::unique_ptr<Water>> &Water,
                 const std::vector<std::shared_ptr<Tank>> &NPCs) {
    const Dir dir{m_dir}; // save true direction
    const bool isMoving{m_type >= basic || g_up || g_left || g_down || g_right};

    for (int i{0}; i < 4; ++i) {
      m_dir = static_cast<Dir>(i);

      if (((isMoving && m_dir == dir) || m_slide[i]) && !checkOutOfBounds() &&
          !checkCollision(Walls) && !checkCollision(Water) &&
          !(checkCollision(NPCs))) {
        if (m_dir == up) {
          m_y -= m_speed;
          if (isMoving && m_dir == dir) {
            m_dirRestrict = {1, 1, 1, 1};
            ++m_anim;
          }

        } else if (m_dir == left) {
          m_x -= m_speed;
          if (isMoving && m_dir == dir) {
            m_dirRestrict = {1, 1, 1, 1};
            ++m_anim;
          }

        } else if (m_dir == down) {
          m_y += m_speed;
          if (isMoving && m_dir == dir) {
            m_dirRestrict = {1, 1, 1, 1};
            ++m_anim;
          }

        } else if (m_dir == right) {
          m_x += m_speed;
          if (isMoving && m_dir == dir) {
            ++m_anim;
            m_dirRestrict = {1, 1, 1, 1};
          }
        }
      }
      if (m_upgrade && m_anim > 5 * constants::refreshRate)
        m_upgrade = false;
    }

    m_dir = dir; // restore true direction
  }

  // getProjectile {{{1
  // Get speed and coordinates for new projectile
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
      speed = 2;
      break;
    case fast:
      speed = 4;
      break;
    case power:
      speed = 4;
      break;
    case armor:
      speed = 3;
      break;
    }

    switch (m_dir) {
    case up:
      return {m_x + m_width / 2 - 1, m_y - 4, m_dir, speed};
    case left:
      return {m_x - 4, m_y + m_width / 2 - 1, m_dir, speed};
    case down:
      return {m_x + m_width / 2 - 1, m_y + m_length, m_dir, speed};
    case right:
      return {m_x + m_length, m_y + m_width / 2 - 1, m_dir, speed};
    }
    return {};
  }

  // onSpawn {{{1
  // Check if tank occupies spawn location
  bool onSpawn(int spot) const {
    switch (spot) {
    case 1:
      return m_y < 16 && m_x < 16;
    case 2:
      return m_y < 16 && m_x > 80 && m_x < 112;
    case 3:
      return m_y < 16 && m_x > 176;
    }
    return false;
  }

  // updateDir {{{1
  // Decide whether to turn
  void updateDir(const std::vector<int> &rng) {
    m_dirProb = {m_dirRestrict[up],
                 (m_x - 112 + rng[left]) * m_dirRestrict[left],
                 (176 - m_y + rng[down]) * m_dirRestrict[down],
                 (80 - m_x + rng[right]) * m_dirRestrict[right]};
    if (m_nextTurn <= 0 && rng[4] > 10) {
      m_dir = static_cast<Dir>(
          std::distance(m_dirProb.begin(),
                        std::max_element(m_dirProb.begin(), m_dirProb.end())));
      m_nextTurn =
          constants::refreshRate * constants::npcTurnFrequency / m_speed;
    } else {
      --m_nextTurn;
    }
  }
};

// initTankSprites {{{1
inline std::vector<std::unique_ptr<sf::Sprite>>
initTankSprites(const sf::Texture &Textures) {
  int x{};
  int y{};
  int dX{};
  int dY{};
  int ofX{};
  int ofY{};
  std::vector<std::unique_ptr<sf::Sprite>> objects{};

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

          case colonel:
            switch (dir) {
            case up:
              dX = 13;
              dY = 15;
              ofX = 1;
              ofY = 0;
              break;
            case left:
              dX = 15;
              dY = 13;
              ofX = 0;
              ofY = 1;
              break;
            case down:
              dX = 13;
              dY = 15;
              ofX = 1;
              ofY = 1;
              break;
            case right:
              dX = 15;
              dY = 13;
              ofX = 1;
              ofY = 1;
              break;
            }
            break;

          case general:
            switch (dir) {
            case up:
              dX = 14;
              dY = 15;
              ofX = 1;
              ofY = 0;
              break;
            case left:
              dX = 15;
              dY = 14;
              ofX = 0;
              ofY = 1;
              break;
            case down:
              dX = 14;
              dY = 15;
              ofX = 1;
              ofY = 0;
              break;
            case right:
              dX = 15;
              dY = 14;
              ofX = 1;
              ofY = 1;
              break;
            }
            break;

          case basic:
            switch (dir) {
            case up:
              dX = 13;
              dY = 15;
              ofX = 1;
              ofY = 0;
              break;
            case left:
              dX = 15;
              dY = 13;
              ofX = 0;
              ofY = 2;
              break;
            case down:
              dX = 13;
              dY = 15;
              ofX = 1;
              ofY = 0;
              break;
            case right:
              dX = 15;
              dY = 13;
              ofX = 1;
              ofY = 1;
              break;
            }
            break;

          case fast:
            switch (dir) {
            case up:
              dX = 13;
              dY = 15;
              ofX = 1;
              ofY = 0;
              break;
            case left:
              dX = 15;
              dY = 13;
              ofX = 0;
              ofY = 2;
              break;
            case down:
              dX = 13;
              dY = 15;
              ofX = 1;
              ofY = 1;
              break;
            case right:
              dX = 15;
              dY = 13;
              ofX = 1;
              ofY = 1;
              break;
            }
            break;

          case power:
            switch (dir) {
            case up:
              dX = 13;
              dY = 15;
              ofX = 1;
              ofY = 0;
              break;
            case left:
              dX = 15;
              dY = 13;
              ofX = 0;
              ofY = 2;
              break;
            case down:
              dX = 13;
              dY = 15;
              ofX = 0;
              ofY = 0;
              break;
            case right:
              dX = 15;
              dY = 13;
              ofX = 1;
              ofY = 1;
              break;
            }
            break;

          case armor:
            switch (dir) {
            case up:
              dX = 13;
              dY = 15;
              ofX = 1;
              ofY = 0;
              break;
            case left:
              dX = 15;
              dY = 13;
              ofX = 0;
              ofY = 1;
              break;
            case down:
              dX = 13;
              dY = 15;
              ofX = 1;
              ofY = 0;
              break;
            case right:
              dX = 15;
              dY = 13;
              ofX = 1;
              ofY = 1;
              break;
            }
            break;
          }

          x = dir * 32 + color % 2 * 128 + anim * 16;
          y = type * 16 + color / 2 * 128;
          objects.emplace_back(std::make_unique<sf::Sprite>(
              Textures, sf::IntRect({x + ofX, y + ofY}, {dX, dY})));
        }
      }
    }
  }
  return objects;
};

// initImmunitySprites {{{1
inline std::vector<std::unique_ptr<sf::Sprite>>
initImmunitySprites(const sf::Texture &Textures) {
  std::vector<std::unique_ptr<sf::Sprite>> objects{};
  objects.emplace_back(std::make_unique<sf::Sprite>(
      Textures, sf::IntRect({256, 144}, {16, 16})));
  objects.emplace_back(std::make_unique<sf::Sprite>(
      Textures, sf::IntRect({272, 144}, {16, 16})));
  return objects;
};
// }}}1
