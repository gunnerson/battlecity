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
  sf::Texture m_texture1{};
  sf::Texture m_texture2{};
  sf::Texture m_texture3{};
  sf::Texture m_texture4{};
  sf::Texture m_texture5{};

public:
  Projectile(int x, int y, int dir, const sf::Image &Sprites)
      : m_x{x}, m_y{y}, m_dir{dir},
        m_texture1{
            sf::Texture(Sprites, false, sf::IntRect({256, 96}, {16, 16}))},
        m_texture2{
            sf::Texture(Sprites, false, sf::IntRect({272, 96}, {16, 16}))},
        m_texture3{
            sf::Texture(Sprites, false, sf::IntRect({288, 96}, {16, 16}))},
        m_texture4{
            sf::Texture(Sprites, false, sf::IntRect({304, 96}, {16, 16}))},
        m_texture5{sf::Texture(Sprites, false,
                               sf::IntRect({320 + dir * 8, 100}, {8, 8}))} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }

  void move() {
    switch (m_dir) {
    case 0:
      --m_y;
      break;
    case 1:
      --m_x;
      break;
    case 2:
      ++m_y;
      break;
    case 3:
      ++m_x;
      break;
    }
    ++m_anim;
  }

  sf::Texture getTexture() const {
    if (m_anim < g_updateRate * 0.05)
      return m_texture1;
    else if (m_anim < g_updateRate * 0.1)
      return m_texture2;
    else if (m_anim < g_updateRate * 0.15)
      return m_texture3;
    else if (m_anim < g_updateRate * 0.2)
      return m_texture4;
    else
      return m_texture5;
  };
};
