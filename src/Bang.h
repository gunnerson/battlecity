#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <vector>

extern const int g_updateRate;

class Bang {
private:
  int m_x{};
  int m_y{};
  int m_anim{0};

public:
  Bang(int x, int y) : m_x{x}, m_y{y} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }
  int getAnim() const { return m_anim; }

  bool is_alive() const { return m_anim < g_updateRate * 3 / 5 - 1; }
  void anim() { ++m_anim; }

  std::shared_ptr<sf::Texture>
  getTexture(const std::vector<std::shared_ptr<sf::Texture>> &Textures) const {
    return Textures[m_anim * 5 / g_updateRate];
  };
};

inline std::vector<std::shared_ptr<sf::Texture>>
initBangTextures(const sf::Image &Sprites) {
  std::vector<std::shared_ptr<sf::Texture>> objects{};
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({304, 128}, {32, 32})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({336, 128}, {32, 32})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({304, 128}, {32, 32})));
  return objects;
};
