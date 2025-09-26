#pragma once
#include <SFML/Graphics/Texture.hpp>

class GameOver {
private:
  int m_x{88};
  int m_y{192};
  bool m_alive{false};

public:
  int getX() const { return m_x; }
  int getY() const { return m_y; }
  bool is_alive() const { return m_alive; }
  void setAlive() { m_alive = true; }
  void anim() {
    if (m_y > 104)
      --m_y;
  }

  std::shared_ptr<sf::Texture> getTexture(const sf::Image &Sprites) const {
    return std::make_shared<sf::Texture>(Sprites, false,
                                         sf::IntRect({289, 184}, {31, 15}));
  }
};
