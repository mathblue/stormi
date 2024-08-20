#ifndef BOID_RENDERER
#define BOID_RENDERER

#include <SFML/Graphics.hpp>
#include "boid.hpp"
#include <vector>

class BoidRenderer {
private:
    sf::RenderWindow window;
    sf::CircleShape boidShape;
    double scaleX;
    double scaleY;

public:
    BoidRenderer(int width, int height, const std::string& title);
    void render(const std::vector<Boid>& boids);
    bool isOpen() const;
    void processEvents();
};

#endif 
