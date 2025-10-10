#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class GameOver {
private:
  int m_x{88};
  int m_y{192};
  bool m_alive{false};

public:
  int getX() const { return m_x; }
  int getY() const { return m_y; }
  bool isAlive() const { return m_alive; }
  void setAlive() { m_alive = true; }
  void kill() { m_alive = false; }
  void anim() {
    if (m_y > 104)
      --m_y;
  }

  std::unique_ptr<sf::Sprite> getSprite(const sf::Texture &Textures) const {
    return std::make_unique<sf::Sprite>(Textures,
                                        sf::IntRect({289, 184}, {31, 15}));
  }
};
