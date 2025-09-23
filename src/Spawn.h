#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <vector>

class Spawn {
private:
  int m_x{};
  int m_y{};
  int m_anim{0};

public:
  Spawn(int x, int y) : m_x{x}, m_y{y} {}

  int getX() const { return m_x; }
  int getY() const { return m_y; }

  bool is_alive() const { return m_anim < 4; }
  void anim() { ++m_anim; }

  std::shared_ptr<sf::Texture>
  getTexture(const std::vector<std::shared_ptr<sf::Texture>> &Textures) const {
    return Textures[m_anim];
  };
};

inline std::vector<std::shared_ptr<sf::Texture>>
initSpawnTextures(const sf::Image &Sprites) {
  std::vector<std::shared_ptr<sf::Texture>> objects{};
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({259, 99}, {9, 9})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({274, 98}, {11, 11})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({289, 97}, {13, 13})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({304, 96}, {15, 15})));
  return objects;
};
