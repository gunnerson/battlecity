//
// imports {{{1
#include "BrickWall.h"
#include "Projectile.h"
#include "Tank.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>

// globals {{{1
constexpr int g_maxX = 208;
constexpr int g_maxY = 208;
constexpr int g_updateRate = 30;
bool g_up = false;
bool g_left = false;
bool g_down = false;
bool g_right = false;

// events {{{1
void handleUpKeyPressed(Tank *tank) {
  tank->setDir(0);
  g_up = true;
  g_left = g_right = g_down = false;
}
void handleLeftKeyPressed(Tank *tank) {
  tank->setDir(1);
  g_left = true;
  g_up = g_right = g_down = false;
}
void handleDownKeyPressed(Tank *tank) {
  tank->setDir(2);
  g_down = true;
  g_up = g_left = g_right = false;
}
void handleRightKeyPressed(Tank *tank) {
  tank->setDir(3);
  g_right = true;
  g_up = g_left = g_down = false;
}
void handleUpKeyReleased(Tank *tank) { g_up = false; }
void handleLeftKeyReleased(Tank *tank) { g_left = false; }
void handleDownKeyReleased(Tank *tank) { g_down = false; }
void handleRightKeyReleased(Tank *tank) { g_right = false; }
void handleSpaceKeyPressed(
    Tank *tank, std::vector<std::unique_ptr<Projectile>> &Projectiles) {
  auto [x, y, dir] = tank->getProjectilePos();
  auto obj = std::make_unique<Projectile>(x, y, dir);
  Projectiles.push_back(std::move(obj));
}

// main {{{1
int main() {
  auto window =
      sf::RenderWindow(sf::VideoMode({static_cast<unsigned int>(g_maxX),
                                      static_cast<unsigned int>(g_maxY)}),
                       "Battlecity");
  window.setFramerateLimit(g_updateRate);
  window.setKeyRepeatEnabled(false);

  const sf::Image Sprites("res/sprites.png");

  // User tank
  std::vector<std::unique_ptr<sf::Texture>> TankTextures =
      initTankTextures(Sprites);
  auto userTank{std::make_unique<Tank>(0, 1, 1, 0, 64, 192, TankTextures)};
  auto userTank_ptr = userTank.get();
  sf::Texture usertankTexture = userTank->getTexture();
  sf::Sprite userTankSprite(usertankTexture);

  // Brick walls
  int brickWallsN = sizeof(brickWallsArray) / sizeof(brickWallsArray[0]);
  std::vector<std::unique_ptr<BrickWall>> BrickWalls{};
  for (int i{0}; i < brickWallsN; ++i) {
    auto obj = std::make_unique<BrickWall>(brickWallsArray[i][0],
                                           brickWallsArray[i][1],
                                           brickWallsArray[i][2], Sprites);
    BrickWalls.push_back(std::move(obj));
  }
  sf::Texture brickWallTexture{BrickWalls[0]->getTexture()};
  sf::Sprite brickWallSprite(brickWallTexture);

  // Projectiles
  std::vector<std::unique_ptr<sf::Texture>> ProjectileTextures =
      initProjectileTextures(Sprites);
  std::vector<std::unique_ptr<Projectile>> Projectiles{};

  // Handle events
  const auto onClose = [&window](const sf::Event::Closed &) { window.close(); };

  const auto onKeyPressed = [&window, &userTank_ptr, &Projectiles](
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
  };

  const auto onKeyReleased =
      [&window, &userTank_ptr](const sf::Event::KeyReleased &keyReleased) {
        if (keyReleased.scancode == sf::Keyboard::Scancode::Up)
          handleUpKeyReleased(userTank_ptr);
        else if (keyReleased.scancode == sf::Keyboard::Scancode::Left)
          handleLeftKeyReleased(userTank_ptr);
        else if (keyReleased.scancode == sf::Keyboard::Scancode::Down)
          handleDownKeyReleased(userTank_ptr);
        else if (keyReleased.scancode == sf::Keyboard::Scancode::Right)
          handleRightKeyReleased(userTank_ptr);
      };

  // Main cycle
  while (window.isOpen()) {
    window.handleEvents(onClose, onKeyPressed, onKeyReleased);
    window.clear();

    // Draw projectiles
    for (const auto &obj : Projectiles) {
      auto texture = obj->getTexture(ProjectileTextures);
      sf::Sprite sprite(texture);
      sprite.setPosition(
          {static_cast<float>(obj->getX()), static_cast<float>(obj->getY())});
      window.draw(sprite);
      obj->move();
    }

    // Draw user tank
    if (g_up || g_left || g_down || g_right) {
      userTank->updatePos(BrickWalls);
      userTank->setTexture(TankTextures);
    }
    usertankTexture = userTank->getTexture();
    userTankSprite.setTexture(usertankTexture);
    userTankSprite.setPosition({static_cast<float>(userTank->getX()),
                                static_cast<float>(userTank->getY())});
    window.draw(userTankSprite);

    // Draw brick walls
    for (const auto &obj : BrickWalls) {
      if (obj->is_alive()) {
        brickWallTexture = obj->getTexture();
        brickWallSprite.setTexture(brickWallTexture);
        brickWallSprite.setPosition(
            {static_cast<float>(obj->getX()), static_cast<float>(obj->getY())});
        window.draw(brickWallSprite);
      }
    }

    window.display();
  }
}
