#ifndef BOID_RENDERER
#define BOID_RENDERER

#include "boid.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class BoidRenderer
{
 private:
  sf::RenderWindow window;
  sf::CircleShape boidShape;
  double scaleX;
  double scaleY;

 public:
  BoidRenderer(unsigned int const& width, unsigned int const& height,
               const std::string& title);
  void render(const std::vector<Boid>& boids, const std::string& info);
  bool isOpen() const;
  void processEvents();

  sf::Font font;
  sf::Text infoText;
};

#endif
