//
// imports {{{1
#include "Base.h"
#include "GameOver.h"
#include "Hit.h"
#include "Projectile.h"
#include "Tank.h"
#include "Wall.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>

// globals {{{1
constexpr int g_maxX{208};
constexpr int g_maxY{208};
constexpr int g_updateRate{30};
int g_stage{0};
bool g_gameOver{false};
bool g_up{false};
bool g_left{false};
bool g_down{false};
bool g_right{false};

// events {{{1
void handleUpKeyPressed(Tank *tank) {
  if (!g_gameOver) {
    tank->setDir(0);
    g_up = true;
    g_left = g_right = g_down = false;
  }
}
void handleLeftKeyPressed(Tank *tank) {
  if (!g_gameOver) {
    tank->setDir(1);
    g_left = true;
    g_up = g_right = g_down = false;
  }
}
void handleDownKeyPressed(Tank *tank) {
  if (!g_gameOver) {
    tank->setDir(2);
    g_down = true;
    g_up = g_left = g_right = false;
  }
}
void handleRightKeyPressed(Tank *tank) {
  if (!g_gameOver) {
    tank->setDir(3);
    g_right = true;
    g_up = g_left = g_down = false;
  }
}
void handleUpKeyReleased() { g_up = false; }
void handleLeftKeyReleased() { g_left = false; }
void handleDownKeyReleased() { g_down = false; }
void handleRightKeyReleased() { g_right = false; }
void handleSpaceKeyPressed(
    Tank *tank, std::vector<std::unique_ptr<Projectile>> &Projectiles) {
  if (!g_gameOver && !tank->is_reloading()) {
    auto [x, y, dir] = tank->getProjectilePos();
    Projectiles.emplace_back(std::make_unique<Projectile>(x, y, dir));
    tank->startReload();
  }
}

// main {{{1
int main() {
  auto window{
      sf::RenderWindow(sf::VideoMode({static_cast<unsigned int>(g_maxX),
                                      static_cast<unsigned int>(g_maxY)}),
                       "Battlecity")};
  window.setFramerateLimit(g_updateRate);
  window.setKeyRepeatEnabled(false);

  const sf::Image Sprites("res/sprites.png");

  // Base
  const auto base{std::make_unique<Base>()};
  const std::vector<std::shared_ptr<sf::Texture>> BaseTextures{
      initBaseTextures(Sprites)};
  auto baseTexture{base->getTexture(BaseTextures)};
  auto baseSprite{sf::Sprite(*baseTexture)};

  // GameOver
  const auto gameOver{std::make_unique<GameOver>()};
  auto gameOverTexture{gameOver->getTexture(Sprites)};
  auto gameOverSprite{sf::Sprite(*gameOverTexture)};

  // User tank
  const std::vector<std::shared_ptr<sf::Texture>> TankTextures{
      initTankTextures(Sprites)};
  const auto userTank{
      std::make_unique<Tank>(0, 1, 0, 0, 64, 195, TankTextures)};
  const auto userTank_ptr{userTank.get()};
  std::shared_ptr<sf::Texture> usertankTexture{
      userTank->getTexture(TankTextures)};
  sf::Sprite userTankSprite(*usertankTexture);

  // Walls
  const std::vector<std::shared_ptr<sf::Texture>> WallTextures{
      initWallTextures(Sprites)};
  std::vector<std::unique_ptr<Wall>> Walls{initWalls()};
  std::shared_ptr<sf::Texture> wallTexture{Walls[0]->getTexture(WallTextures)};
  sf::Sprite wallSprite(*wallTexture);

  // Projectiles
  const std::vector<std::shared_ptr<sf::Texture>> ProjectileTextures{
      initProjectileTextures(Sprites)};
  std::vector<std::unique_ptr<Projectile>> Projectiles{};

  // Hits
  const std::vector<std::shared_ptr<sf::Texture>> HitTextures{
      initHitTextures(Sprites)};
  std::vector<std::unique_ptr<Hit>> Hits{};

  // Handle events
  const auto onClose{[&window](const sf::Event::Closed &) { window.close(); }};

  const auto onKeyPressed{[&window, &userTank_ptr, &Projectiles](
                              const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scancode::Escape)
      window.close();
    else if (keyPressed.scancode == sf::Keyboard::Scancode::Up)
      handleUpKeyPressed(userTank_ptr);
    else if (keyPressed.scancode == sf::Keyboard::Scancode::Left)
      handleLeftKeyPressed(userTank_ptr);
    else if (keyPressed.scancode == sf::Keyboard::Scancode::Down)
      handleDownKeyPressed(userTank_ptr);
    else if (keyPressed.scancode == sf::Keyboard::Scancode::Right)
      handleRightKeyPressed(userTank_ptr);
    else if (keyPressed.scancode == sf::Keyboard::Scancode::Space)
      handleSpaceKeyPressed(userTank_ptr, Projectiles);
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

  // Main cycle
  while (window.isOpen()) {
    window.handleEvents(onClose, onKeyPressed, onKeyReleased);
    window.clear();

    // Draw base
    baseTexture = base->getTexture(BaseTextures);
    baseSprite.setTexture(*baseTexture);
    baseSprite.setPosition(
        {static_cast<float>(base->getX()), static_cast<float>(base->getY())});
    window.draw(baseSprite);

    // Draw projectiles
    for (std::size_t i{0}; i < Projectiles.size(); ++i) {
      Projectiles[i]->move();
      if (!g_gameOver && Projectiles[i]->checkBaseHit()) {
        g_gameOver = true;
        auto [x, y, dir]{Projectiles[i]->getHitPos()};
        Hits.emplace_back(std::make_unique<Hit>(x, y, dir));
        Projectiles.erase(Projectiles.begin() + i);
        base->kill();
      } else {
        auto texture{Projectiles[i]->getTexture(ProjectileTextures)};
        sf::Sprite sprite(*texture);
        sprite.setPosition({static_cast<float>(Projectiles[i]->getX()),
                            static_cast<float>(Projectiles[i]->getY())});
        window.draw(sprite);
        for (const auto &wall : Walls) {
          if (wall->is_alive()) {
            if (Projectiles[i]->checkCollision(wall->getX(), wall->getY())) {
              auto [x, y, dir]{Projectiles[i]->getHitPos()};
              Hits.emplace_back(std::make_unique<Hit>(x, y, dir));
              Projectiles.erase(Projectiles.begin() + i);
              break;
            }
          }
        }
      }
    }

    // Draw user tank
    if (!g_gameOver && (g_up || g_left || g_down || g_right)) {
      userTank->updatePos(Walls);
    }
    userTank->reload();
    usertankTexture = userTank->getTexture(TankTextures);
    userTankSprite.setTexture(*usertankTexture);
    userTankSprite.setPosition({static_cast<float>(userTank->getX()),
                                static_cast<float>(userTank->getY())});
    window.draw(userTankSprite);

    // Draw brick walls
    for (const auto &wall : Walls) {
      if (wall->is_alive()) {
        wallTexture = wall->getTexture(WallTextures);
        wallSprite.setTexture(*wallTexture);
        wallSprite.setPosition({static_cast<float>(wall->getX()),
                                static_cast<float>(wall->getY())});
        window.draw(wallSprite);
      }
    }

    // Draw hits
    for (std::size_t i{0}; i < Hits.size(); ++i) {
      if (Hits[i]->is_alive()) {
        auto hitTexture{Hits[i]->getTexture(HitTextures)};
        auto hitSprite{sf::Sprite(*hitTexture)};
        hitSprite.setPosition({static_cast<float>(Hits[i]->getX()),
                               static_cast<float>(Hits[i]->getY())});
        window.draw(hitSprite);
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

    // Draw base
    if (g_gameOver) {
      gameOverSprite.setPosition({static_cast<float>(gameOver->getX()),
                                  static_cast<float>(gameOver->getY())});
      window.draw(gameOverSprite);
      gameOver->anim();
    }

    // Clean projectiles
    for (std::size_t i{0}; i < Projectiles.size(); ++i) {
      if (Projectiles[i]->is_out()) {
        Projectiles.erase(Projectiles.begin() + i);
      }
    }

    // Clean brick walls
    for (std::size_t i{0}; i < Walls.size(); ++i) {
      if (!Walls[i]->is_alive()) {
        Walls.erase(Walls.begin() + i);
      }
    }

    // Draw Base

    window.display();
  }
}
