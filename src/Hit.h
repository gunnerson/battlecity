#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

extern const int g_updateRate;

class Hit {
private:
  int m_x{};
  int m_y{};
  int m_anim{0};

public:
  Hit(int x, int y) : m_x{x}, m_y{y} {}

  sf::Texture
  getTexture(const std::vector<std::unique_ptr<sf::Texture>> &Textures) const {
    return *Textures[m_anim];
  };
};

inline std::vector<std::unique_ptr<sf::Texture>>
initHitTextures(const sf::Image &Sprites) {
  std::vector<std::unique_ptr<sf::Texture>> objects{};
  objects.emplace_back(std::make_unique<sf::Texture>(
      Sprites, false, sf::IntRect({259, 99}, {9, 9})));
  objects.emplace_back(std::make_unique<sf::Texture>(
      Sprites, false, sf::IntRect({274, 98}, {11, 11})));
  objects.emplace_back(std::make_unique<sf::Texture>(
      Sprites, false, sf::IntRect({289, 97}, {13, 13})));
  objects.emplace_back(std::make_unique<sf::Texture>(
      Sprites, false, sf::IntRect({304, 96}, {15, 15})));
  return objects;
};
