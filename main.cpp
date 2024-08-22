#include "BoidRenderer.hpp"
#include "boid.hpp"
#include "config.hpp"
#include "helper.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

std::string getInfoString(std::vector<Boid> const& boids)
{
    std::ostringstream oss;
    
    auto [avgDistance, stdDevDistance] = averageDistanceAndStdDev(boids);
    oss << "Avg. Distance: " << avgDistance
        << " std. dev.:" << stdDevDistance << "\n";

    auto [avgVelocity, stdDevVelocity] = averageVelocityAndStdDev(boids);
    oss << "Avg. Velocity: (" << avgVelocity.first << ", " << avgVelocity.second << ")\n"
        << "Std. Dev. of Velocity: (" << stdDevVelocity.first << ", " << stdDevVelocity.second << ")";

    return oss.str();
}
// Let's define an alias for shorter usage, cannot be modified
const Config& cfg = Config::getInstance();

int main()
{
  // Load the configuration once, the cast allows us to make it modifiable.
  Config& config = const_cast<Config&>(Config::getInstance());
  config.loadConfig("config.txt");

  BoidRenderer renderer(1500, 1000, "Boids Simulation");

  std::vector<Boid> boids =
      generateBoids(cfg.getNBoids(), cfg.getXSpace()[0], cfg.getXSpace()[1],
                    cfg.getYSpace()[0], cfg.getYSpace()[1]);

  lazyUpdateNeighbors(boids);

  std::string info=getInfoString(boids);

  for (double time = 0; time < cfg.getTTot(); time += cfg.getDt()) {
    renderer.processEvents();

    std::for_each(boids.begin(), boids.end(), [](Boid& b) {
      b.updateVelocity();
      b.updatePosition();
    });

    lazyUpdateNeighbors(boids);

    renderer.render(boids, info);

    // print info every 10s
    if (static_cast<int>(time / cfg.getDt())
            % static_cast<int>(1 / cfg.getDt())
        == 0) {
      info = getInfoString(boids);
    }

    if (!renderer.isOpen()) {
      break;
    }
  }
}