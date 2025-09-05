//
// imports {{{1
#include "BrickWall.h"
#include "Tank.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>

// globals {{{1
constexpr int g_maxX = 208;
constexpr int g_maxY = 208;

// events {{{1
void handleUpKeyPressed(Tank *tank) { tank->setDir(0); }
void handleRightKeyPressed(Tank *tank) { tank->setDir(3); }
void handleDownKeyPressed(Tank *tank) { tank->setDir(2); }
void handleLeftKeyPressed(Tank *tank) { tank->setDir(1); }
void handleSpaceKeyPressed() {}

// main {{{1
int main() {
  auto window =
      sf::RenderWindow(sf::VideoMode({static_cast<unsigned int>(g_maxX),
                                      static_cast<unsigned int>(g_maxY)}),
                       "Battlecity");
  window.setFramerateLimit(30);
  window.setKeyRepeatEnabled(false);

  const sf::Image Sprites("res/sprites.png");

  // User tank sprite
  auto userTank{std::make_unique<Tank>(0, 1, 1, 0, 64, 192)};
  auto userTank_ptr = userTank.get();
  sf::Texture usertankTexture = userTank->getTexture(Sprites);
  sf::Sprite userTankSprite(usertankTexture);

  // Brick walls
  int brickWallsN = sizeof(brickWallsArray) / sizeof(brickWallsArray[0]);
  std::vector<std::unique_ptr<BrickWall>> BrickWalls;
  for (int i{0}; i < brickWallsN; ++i) {
    auto obj = std::make_unique<BrickWall>(brickWallsArray[i][0],
                                           brickWallsArray[i][1],
                                           brickWallsArray[i][2], Sprites);
    BrickWalls.push_back(std::move(obj));
  }
  sf::Texture brickWallTexture{BrickWalls[0]->getTexture()};
  sf::Sprite brickWallSprite(brickWallTexture);

  const auto onClose = [&window](const sf::Event::Closed &) { window.close(); };

  const auto onKeyPressed =
      [&window, &userTank_ptr](const sf::Event::KeyPressed &keyPressed) {
        if (keyPressed.scancode == sf::Keyboard::Scancode::Escape)
          window.close();
        else if (keyPressed.scancode == sf::Keyboard::Scancode::Up)
          handleUpKeyPressed(userTank_ptr);
        else if (keyPressed.scancode == sf::Keyboard::Scancode::Down)
          handleDownKeyPressed(userTank_ptr);
        else if (keyPressed.scancode == sf::Keyboard::Scancode::Left)
          handleLeftKeyPressed(userTank_ptr);
        else if (keyPressed.scancode == sf::Keyboard::Scancode::Right)
          handleRightKeyPressed(userTank_ptr);
        else if (keyPressed.scancode == sf::Keyboard::Scancode::Space)
          handleSpaceKeyPressed();
      };

  while (window.isOpen()) {
    window.handleEvents(onClose, onKeyPressed);
    window.clear();

    // Draw user tank
    userTank->updatePos(BrickWalls);
    usertankTexture = userTank->getTexture(Sprites);
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
