//
// imports {{{1
#include "Bang.h"
#include "Base.h"
#include "Enums.h"
#include "GameOver.h"
#include "Hit.h"
#include "Projectile.h"
#include "Tank.h"
#include "Upgrade.h"
#include "Wall.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

// globals {{{1
const int g_maxX{208}; // Battlefield size, horizontal
const int g_maxY{208}; // Battlefield size, vertical
const int g_ofX{4};    // Frame size, horizontal
const int g_ofY{4};    // Frame size, vertical
const int g_info{32};  // Info panel width
const int g_refreshRate{30};
int g_stage{1};
int g_score{0};
bool g_gameOver{false};
bool g_up{false};
bool g_left{false};
bool g_down{false};
bool g_right{false};

// Event handlers {{{1
static void handleUpKeyPressed(Tank *tank) {
  if (!g_gameOver) {
    tank->setDir(up);
    g_up = true;
    g_left = g_right = g_down = false;
  }
}
static void handleLeftKeyPressed(Tank *tank) {
  if (!g_gameOver) {
    tank->setDir(left);
    g_left = true;
    g_up = g_right = g_down = false;
  }
}
static void handleDownKeyPressed(Tank *tank) {
  if (!g_gameOver) {
    tank->setDir(down);
    g_down = true;
    g_up = g_left = g_right = false;
  }
}
static void handleRightKeyPressed(Tank *tank) {
  if (!g_gameOver) {
    tank->setDir(right);
    g_right = true;
    g_up = g_left = g_down = false;
  }
}
static void handleUpKeyReleased() { g_up = false; }
static void handleLeftKeyReleased() { g_left = false; }
static void handleDownKeyReleased() { g_down = false; }
static void handleRightKeyReleased() { g_right = false; }
static void
handleSpaceKeyPressed(std::shared_ptr<Tank> tank,
                      std::vector<std::unique_ptr<Projectile>> &Projectiles) {
  if (!g_gameOver && tank->canShoot()) {
    auto [x, y, dir, speed]{tank->getProjectile()};
    Projectiles.emplace_back(
        std::make_unique<Projectile>(x, y, dir, speed, tank));
    tank->shoot();
  }
}

// main {{{1
int main() {
  // Initializations {{{2
  // Window {{{3
  auto window{sf::RenderWindow(
      sf::VideoMode({static_cast<unsigned int>(g_maxX) + g_ofX + g_info,
                     static_cast<unsigned int>(g_maxY) + g_ofY + g_info}),
      "Battlecity")};
  window.setFramerateLimit(g_refreshRate);
  window.setKeyRepeatEnabled(false);
  sf::RectangleShape battlefield(
      {static_cast<unsigned int>(g_maxX), static_cast<unsigned int>(g_maxY)});
  battlefield.setPosition(
      {static_cast<unsigned int>(g_ofX), static_cast<unsigned int>(g_ofY)});
  battlefield.setFillColor(sf::Color::Black);

  // Sprites {{{3
  sf::Texture Textures;
  if (!Textures.loadFromFile("res/sprites.png")) {
    std::cerr << "Error: Could not load texture from file!" << std::endl;
    return EXIT_FAILURE;
  }

  // PRNG {{{3
  std::mt19937 mt{std::random_device{}()};
  std::uniform_int_distribution rng3{1, 3};
  std::uniform_int_distribution rng4{1, 4};
  std::uniform_int_distribution rng100{1, 100};

  // Base {{{3
  const auto base{std::make_unique<Base>()};
  const std::vector<std::unique_ptr<sf::Sprite>> BaseSprites{
      initBaseSprites(Textures)};

  // GameOver {{{3
  const auto gameOver{std::make_unique<GameOver>()};
  const auto gameOverSprite{gameOver->getSprite(Textures)};

  // Upgrades {{{3
  const std::vector<std::unique_ptr<sf::Sprite>> UpgradeSprites{
      initUpgradeSprites(Textures)};
  std::vector<std::unique_ptr<Upgrade>> Upgrades{};
  for (int i{0}; i < 6; ++i) {
    Upgrades.emplace_back(
        std::make_unique<Upgrade>(static_cast<UpgradeType>(i)));
  }

  // Tanks {{{3
  const std::vector<std::unique_ptr<sf::Sprite>> TankSprites{
      initTankSprites(Textures)};
  const std::vector<std::unique_ptr<sf::Sprite>> ImmunitySprites{
      initImmunitySprites(Textures)};
  std::vector<std::shared_ptr<Tank>> Tanks{};
  int nextSpawn{-10 * g_refreshRate};
  int nextNPC{0};

  // Player tank {{{3
  Tanks.emplace_back(std::make_shared<Tank>(player, 64, 192));
  const auto playerTank{Tanks[0]};

  // Walls {{{3
  const std::vector<std::unique_ptr<sf::Sprite>> WallSprites{
      initWallSprites(Textures)};
  std::vector<std::unique_ptr<Wall>> Walls{initWalls()};

  // Projectiles {{{3
  const std::vector<std::unique_ptr<sf::Sprite>> ProjectileSprites{
      initProjectileSprites(Textures)};
  std::vector<std::unique_ptr<Projectile>> Projectiles{};

  // Hits {{{3
  const std::vector<std::unique_ptr<sf::Sprite>> HitSprites{
      initHitSprites(Textures)};
  std::vector<std::unique_ptr<Hit>> Hits{};

  // Bangs {{{3
  const std::vector<std::unique_ptr<sf::Sprite>> BangSprites{
      initBangSprites(Textures)};
  std::vector<std::unique_ptr<Bang>> Bangs{};

  // Score Board {{{3
  sf::Sprite npcLife(Textures, sf::IntRect({321, 193}, {7, 7}));
  sf::Sprite playerLife(Textures, sf::IntRect({377, 136}, {15, 16}));
  sf::Sprite stage(Textures, sf::IntRect({376, 184}, {16, 15}));
  std::vector<std::unique_ptr<sf::Sprite>> NumberSprites{};
  for (int i{0}; i < 11; ++i) {
    NumberSprites.emplace_back(std::make_unique<sf::Sprite>(
        Textures,
        sf::IntRect({329 + 8 * i - 40 * (i > 4), 184 + 8 * (i > 4)}, {7, 7})));
  }

  // Events {{{2
  const auto onClose{[&window](const sf::Event::Closed &) { window.close(); }};
  const auto onResize{[&window](const sf::Event::Resized &) {
    sf::Vector2u size = window.getSize();
    if (size.y < size.x) {
      size.x = size.y;
    } else if (size.x < size.y) {
      size.y = size.x;
    }
    window.setSize(size);
  }};

  const auto onKeyPressed{[&window, &playerTank, &Projectiles](
                              const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scancode::Escape)
      window.close();
    else if (keyPressed.scancode == sf::Keyboard::Scancode::Up)
      handleUpKeyPressed(playerTank.get());
    else if (keyPressed.scancode == sf::Keyboard::Scancode::Left)
      handleLeftKeyPressed(playerTank.get());
    else if (keyPressed.scancode == sf::Keyboard::Scancode::Down)
      handleDownKeyPressed(playerTank.get());
    else if (keyPressed.scancode == sf::Keyboard::Scancode::Right)
      handleRightKeyPressed(playerTank.get());
    else if (keyPressed.scancode == sf::Keyboard::Scancode::Space)
      handleSpaceKeyPressed(playerTank, Projectiles);
  }};

  const auto onKeyReleased{
      [&window](const sf::Event::KeyReleased &keyReleased) {
        if (keyReleased.scancode == sf::Keyboard::Scancode::Up)
          handleUpKeyReleased();
        else if (keyReleased.scancode == sf::Keyboard::Scancode::Left)
          handleLeftKeyReleased();
        else if (keyReleased.scancode == sf::Keyboard::Scancode::Down)
          handleDownKeyReleased();
        else if (keyReleased.scancode == sf::Keyboard::Scancode::Right)
          handleRightKeyReleased();
      }};

  // Main cycle {{{2
  while (window.isOpen()) {
    window.handleEvents(onClose, onKeyPressed, onKeyReleased, onResize);
    window.clear(sf::Color(99, 99, 99));
    window.draw(battlefield);

    // Draw score board
    for (int i{0}; i < 20 - nextNPC; ++i) {
      npcLife.setPosition({static_cast<float>(g_ofX + g_maxX + 4 + (i % 2) * 8),
                           static_cast<float>(g_ofY + 8 * (i / 2))});
      window.draw(npcLife);
    }

    playerLife.setPosition({g_ofX + g_maxX + 4, g_ofY + 112});
    window.draw(playerLife);

    NumberSprites[playerTank->getHealth()]->setPosition(
        {g_ofX + g_maxX + 12, g_ofY + 120});
    window.draw(*NumberSprites[playerTank->getHealth()]);

    stage.setPosition({g_ofX + g_maxX + 4, g_ofY + 160});
    window.draw(stage);
    if (g_stage < 10) {
      NumberSprites[g_stage]->setPosition({g_ofX + g_maxX + 9, g_ofY + 176});
      window.draw(*NumberSprites[g_stage]);
    } else {
      NumberSprites[g_stage / 10]->setPosition(
          {g_ofX + g_maxX + 5, g_ofY + 176});
      NumberSprites[g_stage % 10]->setPosition(
          {g_ofX + g_maxX + 13, g_ofY + 176});
      window.draw(*NumberSprites[g_stage / 10]);
      window.draw(*NumberSprites[g_stage % 10]);
    }

    // Draw base {{{3
    const auto baseSprite{base->getSprite(BaseSprites)};
    baseSprite->setPosition({g_ofX + static_cast<float>(base->getX()),
                             g_ofY + static_cast<float>(base->getY())});
    window.draw(*baseSprite);

    // Draw projectiles {{{3
    for (std::size_t i{0}; i < Projectiles.size(); ++i) {
      // Check if projectile hit the base
      if (!g_gameOver && Projectiles[i]->checkBaseHit()) {
        g_gameOver = true;
        Bangs.emplace_back(std::make_unique<Bang>(88, 176));
        base->kill();
      } else {
        // Check if projectile hit a wall
        if (Projectiles[i]->checkCollision(Walls)) {
          const auto [x, y]{Projectiles[i]->getHitPos()};
          Hits.emplace_back(std::make_unique<Hit>(x, y));
          // Check if projectile hit another projectile
        } else if (Projectiles[i]->checkCollision(Projectiles)) {
          // Check if projectile hit a tank
        } else if (Projectiles[i]->checkCollision(Tanks, Hits)) {
          if (!g_gameOver && !playerTank->is_alive()) {
            g_gameOver = true;
            Bangs.emplace_back(std::make_unique<Bang>(88, 176));
            base->kill();
          }
          // Draw projectile
        } else {
          const auto sprite{Projectiles[i]->getSprite(ProjectileSprites)};
          sprite->setPosition(
              {g_ofX + static_cast<float>(Projectiles[i]->getX()),
               g_ofY + static_cast<float>(Projectiles[i]->getY())});
          window.draw(*sprite);
          Projectiles[i]->move();
        }
      }
    }

    // Draw tanks {{{3
    for (const auto &tank : Tanks) {
      if (tank->is_alive()) {
        const auto tankSprite{tank->getSprite(TankSprites)};
        const auto [dx, dy]{tank->getSize()};
        tankSprite->setPosition({g_ofX + static_cast<float>(tank->getX()),
                                 g_ofY + static_cast<float>(tank->getY())});
        window.draw(*tankSprite);
        const int immunity{tank->getImmunity()};
        if (immunity > 0) {
          const auto immunitySprite{ImmunitySprites[immunity % 4 > 1].get()};
          immunitySprite->setPosition(
              {g_ofX + static_cast<float>(tank->getX() - 1),
               g_ofY + static_cast<float>(tank->getY() - 1)});
          window.draw(*immunitySprite);
        }

        if (tank->getType() < basic) {
          // Player tank
          if (!g_gameOver) {
            if (tank->is_disabled())
              tank->repair();
            else if (g_up || g_left || g_down || g_right) {
              tank->updatePos(Walls, Tanks);
            }
          }
          tank->reload();

        } else {
          // NPCs
          if (tank->is_disabled())
            tank->repair();
          else {
            tank->updatePos(Walls, Tanks);
            if (tank->canShoot() && rng100(mt) > 90) {
              const auto [x, y, dir, speed]{tank->getProjectile()};
              Projectiles.emplace_back(
                  std::make_unique<Projectile>(x, y, dir, speed, tank));
              tank->shoot();
            } else {
              tank->reload();
            }
            tank->updateDir(std::vector<int>{rng100(mt), rng100(mt), rng100(mt),
                                             rng100(mt), rng100(mt)});
          }
        }
      }
    }

    // Draw walls {{{3
    for (const auto &wall : Walls) {
      if (wall->is_alive()) {
        const auto wallSprite = wall->getSprite(WallSprites);
        wallSprite->setPosition({g_ofX + static_cast<float>(wall->getX()),
                                 g_ofY + static_cast<float>(wall->getY())});
        window.draw(*wallSprite);
      }
    }

    // Draw hits {{{3
    for (std::size_t i{0}; i < Hits.size(); ++i) {
      if (Hits[i]->is_alive()) {
        const auto sprite{Hits[i]->getSprite(HitSprites)};
        sprite->setPosition({g_ofX + static_cast<float>(Hits[i]->getX()),
                             g_ofY + static_cast<float>(Hits[i]->getY())});
        window.draw(*sprite);
        Hits[i]->anim();
        if (Hits[i]->getAnim() == 1) {
          for (const auto &wall : Walls) {
            if (Hits[i]->checkBlast(wall->getX(), wall->getY())) {
              wall->kill();
            }
          }
        }
      } else {
        Hits.erase(Hits.begin() + i);
      }
    }

    // Draw bangs {{{3
    for (std::size_t i{0}; i < Bangs.size(); ++i) {
      if (Bangs[i]->is_alive()) {
        const auto sprite{Bangs[i]->getSprite(BangSprites)};
        sprite->setPosition({g_ofX + static_cast<float>(Bangs[i]->getX()),
                             g_ofY + static_cast<float>(Bangs[i]->getY())});
        window.draw(*sprite);
        Bangs[i]->anim();
      } else {
        Bangs.erase(Bangs.begin() + i);
      }
    }

    // Draw GameOver {{{3
    if (g_gameOver) {
      gameOverSprite->setPosition(
          {g_ofX + static_cast<float>(gameOver->getX()),
           g_ofY + static_cast<float>(gameOver->getY())});
      window.draw(*gameOverSprite);
      gameOver->anim();
    }

    // Spawn Tanks {{{3
    if (nextSpawn <= 0) {
      const int spawnSpot{rng3(mt)};
      bool occupied{false};
      for (const auto &tank : Tanks) {
        if (tank->onSpawn(spawnSpot)) {
          occupied = true;
          break;
        }
      }
      if (!occupied && nextNPC < 20) {
        int spawnX{0};
        switch (spawnSpot) {
        case 2:
          spawnX = 97;
          break;
        case 3:
          spawnX = 194;
          break;
        }
        Tanks.emplace_back(
            std::make_unique<Tank>(NPCsArray[g_stage][nextNPC], spawnX, 0));
        ++nextNPC;
        nextSpawn += 5 * g_refreshRate;
      }
    }
    --nextSpawn;

    // Clean tanks {{{3
    for (std::size_t i{0}; i < Tanks.size(); ++i) {
      if (!Tanks[i]->is_alive()) {
        Tanks.erase(Tanks.begin() + i);
      }
    }

    // Clean projectiles {{{3
    for (std::size_t i{0}; i < Projectiles.size(); ++i) {
      if (!Projectiles[i]->is_alive()) {
        Projectiles.erase(Projectiles.begin() + i);
      }
    }

    // Clean brick walls {{{3
    for (std::size_t i{0}; i < Walls.size(); ++i) {
      if (!Walls[i]->is_alive()) {
        Walls.erase(Walls.begin() + i);
      }
    }
    //}}}3

    window.display();
  }
  // }}}2
  std::cout << "Your score is: " << g_score << std::endl;
}
// }}}1
