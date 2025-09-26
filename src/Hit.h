#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <vector>

extern const int g_updateRate;

class Hit {
private:
  int m_x{};
  int m_y{};
  int m_dir{};
  int m_anim{0};

public:
  Hit(int x, int y, int dir) : m_x{x}, m_y{y}, m_dir{dir} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }
  int getAnim() const { return m_anim; }

  bool is_alive() const { return m_anim < g_updateRate * 3 / 10 - 1; }
  void anim() { ++m_anim; }

  bool checkBlast(int x, int y) {
    if (((x == m_x) || (x == m_x + 4) || (x == m_x + 8) || (x == m_x + 12)) &&
        ((y == m_y) || (y == m_y + 4) || (y == m_y + 8) || (y == m_y + 12))) {
      return true;
    }
    return false;
  }

  std::shared_ptr<sf::Texture>
  getTexture(const std::vector<std::shared_ptr<sf::Texture>> &Textures) const {
    return Textures[m_anim * 10 / g_updateRate];
  };
};

inline std::vector<std::shared_ptr<sf::Texture>>
initHitTextures(const sf::Image &Sprites) {
  std::vector<std::shared_ptr<sf::Texture>> objects{};
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({256, 128}, {16, 16})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({272, 128}, {16, 16})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({288, 128}, {16, 16})));
  return objects;
};
