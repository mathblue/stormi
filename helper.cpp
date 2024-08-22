#include "helper.hpp"

std::vector<Boid> generateBoids(int numBoids, double xMin, double xMax,
                                double yMin, double yMax, double vxMin,
                                double vxMax, double vyMin, double vyMax)
{
  std::vector<Boid> boids;

  // It attempts to provide true randomness by accessing a hardware-based source
  // of randomness (if available). If such a source is not available, it falls
  // back on a software-based implementation, which may not be truly random but
  // is still sufficient for seeding purposes.

  std::random_device rd;

  // we seed the pseudo-random generator
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> posXDist(xMin, xMax);
  std::uniform_real_distribution<> posYDist(yMin, yMax);
  std::uniform_real_distribution<> velXDist(vxMin, vxMax);
  std::uniform_real_distribution<> velYDist(vyMin, vyMax);

  for (int i = 0; i < numBoids; ++i) {
    double x  = posXDist(gen);
    double y  = posYDist(gen);
    double vx = velXDist(gen);
    double vy = velYDist(gen);
    // construct dicìrectly in place
    boids.emplace_back(x, y, vx, vy);
  }

  return boids;
}

// I've avoided calling every time vector.size() in for (int i ...) so used
// iterators
void lazyUpdateNeighbors(std::vector<Boid>& boids)
{
  for (auto it1 = boids.begin(); it1 != boids.end(); ++it1) {
    for (auto it2 = std::next(it1); it2 != boids.end(); ++it2) {
      if (it1->inSight(*it2)) {
        it1->addBoid(*it2);
        it2->addBoid(*it1);
      } else {
        it1->removeBoid(*it2);
        it2->removeBoid(*it1);
      }
    }
  }
}

// Do not care about efficiency, it's just a check from time to time
std::pair<double, double>
averageDistanceAndStdDev(std::vector<Boid> const& boids)
{
  double totalDistance        = 0.0;
  double totalDistanceSquared = 0.0;
  int count                   = 0;

  auto it1 = boids.begin();
  while (it1 != boids.end()) {
    auto it2 = it1;
    ++it2;
    while (it2 != boids.end()) {
      double distance = (*it1).distance(*it2);
      totalDistance += distance;
      totalDistanceSquared += distance * distance;
      ++count;
      ++it2;
    }
    ++it1;
  }

  double averageDistance = (count > 0) ? (totalDistance / count) : 0.0;
  double variance        = (count > 1) ? ((totalDistanceSquared / count)
                                   - (averageDistance * averageDistance))
                                       : 0.0;
  double stdDev          = std::sqrt(variance);

  return {averageDistance, stdDev};
}

std::pair<std::pair<double, double>, std::pair<double, double>>
averageVelocityAndStdDev(std::vector<Boid> const& boids)
{
  double totalVx        = 0.0;
  double totalVy        = 0.0;
  double totalVxSquared = 0.0;
  double totalVySquared = 0.0;
  int count             = static_cast<int>(boids.size());

  for (const auto& b : boids) {
    double vx = b.get_vx();
    double vy = b.get_vy();
    totalVx += vx;
    totalVy += vy;
    totalVxSquared += vx * vx;
    totalVySquared += vy * vy;
  }

  double avgVx = (count > 0) ? (totalVx / count) : 0.0;
  double avgVy = (count > 0) ? (totalVy / count) : 0.0;

  double varianceVx =
      (count > 1) ? ((totalVxSquared / count) - (avgVx * avgVx)) : 0.0;
  double varianceVy =
      (count > 1) ? ((totalVySquared / count) - (avgVy * avgVy)) : 0.0;

  double stdDevVx = std::sqrt(varianceVx);
  double stdDevVy = std::sqrt(varianceVy);

  return {{avgVx, avgVy}, {stdDevVx, stdDevVy}};
}