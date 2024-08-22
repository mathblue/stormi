#ifndef HELPER
#define HELPER

#include "boid.hpp"
#include "config.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

std::vector<Boid>
generateBoids(int numBoids, double xMin, double xMax, double yMin, double yMax,
              double vxMin = -Config::getInstance().getVMax() / 4,
              double vxMax = Config::getInstance().getVMax() / 4,
              double vyMin = -Config::getInstance().getVMax() / 4,
              double vyMax = Config::getInstance().getVMax() / 4);

// I've avoided calling every time vector.size() in for (int i ...) so used
// iterators
void lazyUpdateNeighbors(std::vector<Boid>& boids);

std::pair<double, double>
averageDistanceAndStdDev(std::vector<Boid> const& boids);

std::pair<std::pair<double, double>, std::pair<double, double>>
averageVelocityAndStdDev(std::vector<Boid> const& boids);

#endif