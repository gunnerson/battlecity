#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <vector>

class Base {
private:
  int m_x{96};
  int m_y{192};
  bool m_alive{true};

public:
  int getX() const { return m_x; }
  int getY() const { return m_y; }
  bool is_alive() const { return m_alive; }
  void kill() { m_alive = false; }

  std::shared_ptr<sf::Texture>
  getTexture(const std::vector<std::shared_ptr<sf::Texture>> &Textures) const {
    return Textures[m_alive];
  };
};

inline std::vector<std::shared_ptr<sf::Texture>>
initBaseTextures(const sf::Image &Sprites) {
  std::vector<std::shared_ptr<sf::Texture>> objects{};
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({320, 32}, {16, 16})));
  objects.emplace_back(std::make_shared<sf::Texture>(
      Sprites, false, sf::IntRect({304, 32}, {16, 16})));
  return objects;
};
