#include "BoidRenderer.hpp"
#include "config.hpp"
#include <cmath>
#include <iostream>

BoidRenderer::BoidRenderer(int width, int height, const std::string& title)
    : window(sf::VideoMode(width, height), title), boidShape(5) { 
    //window.setFramerateLimit(200); 
    boidShape.setFillColor(sf::Color::White);
    scaleX = width / (cfg.getXSpace()[1] - cfg.getXSpace()[0]);
    scaleY = height / (cfg.getYSpace()[1] - cfg.getYSpace()[0]);
}

void BoidRenderer::render(const std::vector<Boid>& boids) {
    window.clear();

    for (const auto& b : boids) {
        double windowX = (b.get_x() - cfg.getXSpace()[0]) * scaleX;
        double windowY = (b.get_y() - cfg.getYSpace()[0]) * scaleY;

        boidShape.setPosition(windowX, windowY);

        window.draw(boidShape);
    }

    window.display();
}

bool BoidRenderer::isOpen() const {
    return window.isOpen();
}

void BoidRenderer::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}
