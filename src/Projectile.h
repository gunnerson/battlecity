#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

extern const int g_updateRate;

class Projectile {
private:
  int m_x{};
  int m_y{};
  int m_dir{}; // 0 - up, 1 - left, 2 - down, 3 - right
  int m_anim{1};
  int m_speed{3};

public:
  Projectile(int x, int y, int dir) : m_x{x}, m_y{y}, m_dir{dir} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }

  void move() {
    switch (m_dir) {
    case 0:
      m_y -= m_speed;
      break;
    case 1:
      m_x -= m_speed;
      break;
    case 2:
      m_y += m_speed;
      break;
    case 3:
      m_x += m_speed;
      break;
    }
    ++m_anim;
  }

  sf::Texture
  getTexture(const std::vector<std::unique_ptr<sf::Texture>> &Textures) const {
    if (m_anim < g_updateRate * 0.05)
      return *Textures[0];
    else if (m_anim < g_updateRate * 0.1)
      return *Textures[1];
    else if (m_anim < g_updateRate * 0.15)
      return *Textures[2];
    else if (m_anim < g_updateRate * 0.2)
      return *Textures[3];
    else
      return *Textures[4 + m_dir];
  };
};

inline std::vector<std::unique_ptr<sf::Texture>>
initProjectileTextures(const sf::Image &Sprites) {
  std::vector<std::unique_ptr<sf::Texture>> objects{};
  auto obj1 = std::make_unique<sf::Texture>(Sprites, false,
                                            sf::IntRect({259, 99}, {9, 9}));
  objects.push_back(std::move(obj1));
  auto obj2 = std::make_unique<sf::Texture>(Sprites, false,
                                            sf::IntRect({274, 98}, {11, 11}));
  objects.push_back(std::move(obj2));
  auto obj3 = std::make_unique<sf::Texture>(Sprites, false,
                                            sf::IntRect({289, 97}, {13, 13}));
  objects.push_back(std::move(obj3));
  auto obj4 = std::make_unique<sf::Texture>(Sprites, false,
                                            sf::IntRect({304, 96}, {15, 15}));
  objects.push_back(std::move(obj4));
  auto obj5 = std::make_unique<sf::Texture>(Sprites, false,
                                            sf::IntRect({323, 102}, {3, 4}));
  objects.push_back(std::move(obj5));
  auto obj6 = std::make_unique<sf::Texture>(Sprites, false,
                                            sf::IntRect({330, 102}, {4, 3}));
  objects.push_back(std::move(obj6));
  auto obj7 = std::make_unique<sf::Texture>(Sprites, false,
                                            sf::IntRect({339, 102}, {3, 4}));
  objects.push_back(std::move(obj7));
  auto obj8 = std::make_unique<sf::Texture>(Sprites, false,
                                            sf::IntRect({346, 102}, {4, 3}));
  objects.push_back(std::move(obj8));
  return objects;
};
