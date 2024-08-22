#include "BoidRenderer.hpp"
#include "config.hpp"
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

BoidRenderer::BoidRenderer(unsigned int const& width,
                           unsigned int const& height, const std::string& title)
    : window(sf::VideoMode(width, height), title)
    , boidShape(5)
{
  // window.setFramerateLimit(200);
  boidShape.setFillColor(sf::Color::White);
  scaleX = width / (cfg.getXSpace()[1] - cfg.getXSpace()[0]);
  scaleY = height / (cfg.getYSpace()[1] - cfg.getYSpace()[0]);

  if (!font.loadFromFile("./font.ttf")) {
      throw std::runtime_error("Failed to load font");
  }

  infoText.setFont(font);
  infoText.setCharacterSize(14); 
  infoText.setFillColor(sf::Color::White); 
  infoText.setPosition(10.f, 10.f); 
}

void BoidRenderer::render(const std::vector<Boid>& boids, const std::string& info)
{
  window.clear();


  for (const auto& b : boids) {
    float windowX =
        static_cast<float>((b.get_x() - cfg.getXSpace()[0]) * scaleX);
    float windowY =
        static_cast<float>((b.get_y() - cfg.getYSpace()[0]) * scaleY);

    boidShape.setPosition(windowX, windowY);

    window.draw(boidShape);
  }

  
  infoText.setString(info);
  window.draw(infoText);

  window.display();
}

bool BoidRenderer::isOpen() const
{
  return window.isOpen();
}

void BoidRenderer::processEvents()
{
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window.close();
  }
}
