#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "boid.hpp"
#include "config.hpp"
#include "helper.hpp"

#include <cstdio>

const Config& cfg = Config::getInstance();

TEST_CASE("Testing generateBoids function")
{
  Config& config = const_cast<Config&>(Config::getInstance());
  config.loadConfig("config.txt");
  int numBoids = 100;
  double xMin = 0.0, xMax = 100.0;
  double yMin = 0.0, yMax = 100.0;
  double vxMin = -1.0, vxMax = 1.0;
  double vyMin = -1.0, vyMax = 1.0;

  std::vector<Boid> boids = generateBoids(numBoids, xMin, xMax, yMin, yMax,
                                          vxMin, vxMax, vyMin, vyMax);

  REQUIRE(boids.size() == numBoids);

  for (const auto& boid : boids) {
    CHECK(boid.get_x() >= xMin);
    CHECK(boid.get_x() < xMax);
    CHECK(boid.get_y() >= yMin);
    CHECK(boid.get_y() < xMax);
    CHECK(boid.get_vx() >= vxMin);
    CHECK(boid.get_vx() <= vxMax);
    CHECK(boid.get_vy() >= vyMin);
    CHECK(boid.get_vy() <= vyMax);
  }
}

TEST_CASE("Testing averageDistanceAndStdDev function")
{
  Config& config = const_cast<Config&>(Config::getInstance());
  config.loadConfig("config.txt");
  Boid b1(0.0, 0.0, 0.0, 0.0);
  Boid b2(3.0, 4.0, 0.0, 0.0);
  Boid b3(6.0, 8.0, 0.0, 0.0);
  std::vector<Boid> boids = {b1, b2, b3};

  auto [avgDist, stdDev] = averageDistanceAndStdDev(boids);

  CHECK(avgDist == doctest::Approx(6.66667));
  CHECK(stdDev == doctest::Approx(2.35702));
}
TEST_CASE("Testing averageVelocityAndStdDev function")
{
  Config& config = const_cast<Config&>(Config::getInstance());
  config.loadConfig("config.txt");
  Boid b1(0.0, 0.0, 1.0, 2.0);
  Boid b2(0.0, 0.0, 1.0, 1.0);
  Boid b3(0.0, 0.0, 0.0, 3.0);
  std::vector<Boid> boids = {b1, b2, b3};

  auto [avgVel, stdDevVel] = averageVelocityAndStdDev(boids);

  CHECK(avgVel.first == doctest::Approx(0.666667));
  CHECK(avgVel.second == doctest::Approx(2.0));
  CHECK(stdDevVel.first == doctest::Approx(std::sqrt(2) / 3));
  CHECK(stdDevVel.second == doctest::Approx(0.816497));
}

#include <fstream>
#include <stdexcept>

// Utility function to create a temporary configuration file
void createConfigFile(const std::string& filename, const std::string& content)
{
  std::ofstream file(filename);
  if (!file) {
    throw std::runtime_error("Unable to create config file");
  }
  file << content;
}

TEST_CASE("Valid configuration file")
{
  // Create a temporary configuration file with known values
  const std::string configFilename = "valid_config.txt";
  const std::string configContent  = R"(
N_BOIDS=100
dt=0.1
T_TOT=10.0
VMAX=1.0
TorusSpace=true
repulsion=0.5
XSpace0=0.0
XSpace1=10.0
YSpace0=0.0
YSpace1=10.0
d=1.0
d_s=0.5
s=1.5
a=0.1
c=0.2
)";

  createConfigFile(configFilename, configContent);

  try {
    // Load the configuration
    const_cast<Config&>(Config::getInstance()).loadConfig(configFilename);

    // Verify the loaded values
    CHECK(Config::getInstance().getNBoids() == 100);
    CHECK(Config::getInstance().getDt() == doctest::Approx(0.1));
    CHECK(Config::getInstance().getTTot() == doctest::Approx(10.0));
    CHECK(Config::getInstance().getVMax() == doctest::Approx(1.0));
    CHECK(Config::getInstance().isTorusSpace() == true);
    CHECK(Config::getInstance().getRepulsion() == doctest::Approx(0.5));
    CHECK(Config::getInstance().getXSpace()[0] == doctest::Approx(0.0));
    CHECK(Config::getInstance().getXSpace()[1] == doctest::Approx(10.0));
    CHECK(Config::getInstance().getYSpace()[0] == doctest::Approx(0.0));
    CHECK(Config::getInstance().getYSpace()[1] == doctest::Approx(10.0));
    CHECK(Config::getInstance().getD() == doctest::Approx(1.0));
    CHECK(Config::getInstance().getDS() == doctest::Approx(0.5));
    CHECK(Config::getInstance().getS() == doctest::Approx(1.5));
    CHECK(Config::getInstance().getA() == doctest::Approx(0.1));
    CHECK(Config::getInstance().getC() == doctest::Approx(0.2));
  } catch (const std::exception& e) {
    FAIL("Exception thrown during valid config test: " << e.what());
  }

  // Clean up
  std::remove(configFilename.c_str());
}

TEST_CASE("Invalid configuration file - file not found")
{
  // Test loading from a non-existent file
  try {
    CHECK_THROWS_AS(const_cast<Config&>(Config::getInstance())
                        .loadConfig("non_existent_file.txt"),
                    std::runtime_error);
  } catch (const std::exception& e) {
    FAIL("Exception thrown during invalid file test: " << e.what());
  }
}

TEST_CASE("Invalid configuration file - invalid value")
{
  // Create a temporary configuration file with invalid value
  const std::string configFilename = "invalid_config.txt";
  const std::string configContent  = R"(
N_BOIDS=not_a_number
dt=0.1
T_TOT=10.0
VMAX=1.0
TorusSpace=true
repulsion=0.5
XSpace0=0.0
XSpace1=10.0
YSpace0=0.0
YSpace1=10.0
d=1.0
d_s=0.5
s=1.5
a=0.1
c=0.2
)";

  createConfigFile(configFilename, configContent);

  try {
    CHECK_THROWS_AS(
        const_cast<Config&>(Config::getInstance()).loadConfig(configFilename),
        std::runtime_error);
  } catch (const std::exception& e) {
    FAIL("Exception thrown during invalid value test: " << e.what());
  }

  // Clean up
  std::remove(configFilename.c_str());
}

TEST_CASE("Invalid configuration file - missing key")
{
  // Create a temporary configuration file with missing key
  const std::string configFilename = "missing_key_config.txt";
  const std::string configContent  = R"(
N_BOIDS=100
dt=0.1
T_TOT=10.0
VMAX=1.0
TorusSpace=true
repulsion=0.5
XSpace0=0.0
XSpace1=10.0
YSpace0=0.0
YSpace1=10.0
d=1.0
d_s=0.5
s=1.5
a=0.1
)"; // Missing key "c"

  createConfigFile(configFilename, configContent);

  try {
    CHECK_THROWS_AS(
        const_cast<Config&>(Config::getInstance()).loadConfig(configFilename),
        std::runtime_error);
  } catch (const std::exception& e) {
    FAIL("Exception thrown during missing key test: " << e.what());
  }

  // Clean up
  std::remove(configFilename.c_str());
}

void setupConfig()
{
  const std::string configFilename = "valid_config.txt";
  const std::string configContent  = R"(
N_BOIDS=100
dt=0.1
T_TOT=10.0
VMAX=100.0
TorusSpace=true
repulsion=0.5
XSpace0=0.0
XSpace1=25.0
YSpace0=0.0
YSpace1=25.0
d=1.0
d_s=0.5
s=1.5
a=0.1
c=0.2
)";

  createConfigFile(configFilename, configContent);
  const_cast<Config&>(Config::getInstance()).loadConfig(configFilename);
}

TEST_SUITE("Boid Functions Tests")
{
  TEST_CASE("Test mod function")
  {
    setupConfig();

    CHECK(mod(5.0, 10.0) == 5.0);
    CHECK(mod(15.0, 10.0) == 5.0);
    CHECK(mod(-5.0, 10.0) == -5.0);
    CHECK(mod(-15.0, 10.0) == -5.0);
    CHECK(mod(11.0, 10.0) == 1.0);
    CHECK(mod(-11.0, 10.0) == -1.0);
  }

  TEST_CASE("Test separation function")
  {
    setupConfig();

    Boid b1(5.0, 5.0, 0.0, 0.0);
    Boid b2(5.3, 5.3, 0.0, 0.0); // Within d_s
    Boid b3(6.0, 6.0, 0.0, 0.0); // Outside d_s

    std::set<Boid*> neighbors = {&b2, &b3};
    auto result               = separation(b1, neighbors);

    // Calculate the expected separation based on the provided formula
    // Distance between b1 and b2
    double distX = b2.get_x() - b1.get_x(); // 0.3
    double distY = b2.get_y() - b1.get_y(); // 0.3
    // double distance = b1.distance(b2); // sqrt(0.3^2 + 0.3^2) ≈ 0.424

    // Separation force from b2
    double expectedSeparationX =
        -Config::getInstance().getS() * distX; // -1.5 * 0.3 = -0.45
    double expectedSeparationY =
        -Config::getInstance().getS() * distY; // -1.5 * 0.3 = -0.45

    CHECK(result.first == doctest::Approx(expectedSeparationX).epsilon(0.01));
    CHECK(result.second == doctest::Approx(expectedSeparationY).epsilon(0.01));
  }

  TEST_CASE("Test alignment function")
  {
    setupConfig();

    Boid b1(5.0, 5.0, 1.0, 1.0);
    Boid b2(6.0, 6.0, 2.0, 2.0);
    Boid b3(7.0, 7.0, 3.0, 3.0);

    std::set<Boid*> neighbors = {&b2, &b3};
    auto result               = alignment(b1, neighbors);

    // Average velocity of neighbors
    double avgVx = (b2.get_vx() + b3.get_vx()) / 2; // (2.0 + 3.0) / 2 = 2.5
    double avgVy = (b2.get_vy() + b3.get_vy()) / 2; // (2.0 + 3.0) / 2 = 2.5

    // Alignment force
    double expectedAlignmentX =
        Config::getInstance().getA()
        * (avgVx - b1.get_vx()); // 0.1 * (2.5 - 1.0) = 0.15
    double expectedAlignmentY =
        Config::getInstance().getA()
        * (avgVy - b1.get_vy()); // 0.1 * (2.5 - 1.0) = 0.15

    CHECK(result.first == doctest::Approx(expectedAlignmentX).epsilon(0.01));
    CHECK(result.second == doctest::Approx(expectedAlignmentY).epsilon(0.01));
  }

  TEST_CASE("Test cohesion function with toroidal space")
  {
    setupConfig();

    // Assuming the toroidal space is true
    Boid b1(5.0, 5.0, 0.0, 0.0);
    Boid b2(15.0, 15.0, 0.0, 0.0); // Should wrap around
    Boid b3(25.0, 25.0, 0.0, 0.0); // Should wrap around

    std::set<Boid*> neighbors = {&b2, &b3};
    auto result               = cohesion(b1, neighbors);

    // Compute the center of mass considering toroidal wrapping
    double wrapX = Config::getInstance().getXSpace()[1]
                 - Config::getInstance().getXSpace()[0];
    double wrapY = Config::getInstance().getYSpace()[1]
                 - Config::getInstance().getYSpace()[0];

    // we cannot apply the modulo at the end for finding the center of mass

    double centerX = (mod(b2.get_x() - b1.get_x(), wrapX)
                      + mod(b3.get_x() - b1.get_x(), wrapX))
                   / 2;
    double centerY = (mod(b2.get_y() - b1.get_y(), wrapY)
                      + mod(b3.get_y() - b1.get_y(), wrapY))
                   / 2;

    // Cohesion force
    double expectedCohesionX = Config::getInstance().getC() * centerX;
    double expectedCohesionY = Config::getInstance().getC() * centerY;

    CHECK(result.first == doctest::Approx(expectedCohesionX).epsilon(0.01));
    CHECK(result.second == doctest::Approx(expectedCohesionY).epsilon(0.01));
  }
}

TEST_CASE("Test Boid::distance in toroidal space")
{
  const std::string configFilename = "toroidal_config.txt";
  const std::string configContent  = R"(
N_BOIDS=100
dt=0.1
T_TOT=10.0
VMAX=1.0
TorusSpace=true
repulsion=0.5
XSpace0=0.0
XSpace1=10.0
YSpace0=0.0
YSpace1=10.0
d=1.0
d_s=0.5
s=1.5
a=0.1
c=0.2
)";
  createConfigFile(configFilename, configContent);
  const_cast<Config&>(Config::getInstance()).loadConfig(configFilename);

  Boid b1(1.0, 1.0, 0.0, 0.0);
  Boid b2(9.0, 9.0, 0.0, 0.0); // Near edge, should wrap around

  double expectedDistance = std::sqrt(4.0 + 4.0); // Distance after wrapping
  CHECK(b1.distance(b2) == doctest::Approx(expectedDistance).epsilon(0.01));
}

TEST_CASE("Test Boid::distance in non-toroidal space")
{
  // Disable toroidal space
  const std::string configFilename = "non_toroidal_config.txt";
  const std::string configContent  = R"(
N_BOIDS=100
dt=0.1
T_TOT=10.0
VMAX=1.0
TorusSpace=false
repulsion=0.5
XSpace0=0.0
XSpace1=10.0
YSpace0=0.0
YSpace1=10.0
d=1.0
d_s=0.5
s=1.5
a=0.1
c=0.2
)";
  createConfigFile(configFilename, configContent);
  const_cast<Config&>(Config::getInstance()).loadConfig(configFilename);

  Boid b1(1.0, 1.0, 0.0, 0.0);
  Boid b2(9.0, 9.0, 0.0, 0.0);

  double expectedDistance = std::sqrt(64.0 + 64.0); // Euclidean distance
  CHECK(b1.distance(b2) == doctest::Approx(expectedDistance).epsilon(0.01));
}

TEST_CASE("cleanup")
{
  std::remove("valid_config.txt");
  std::remove("non_toroidal_config.txt");
  std::remove("toroidal_config.txt");
}

/*
TEST_CASE("Testing rules"){
    std::vector<Boid>
boids=generateBoids(N_BOIDS,XSpace[0],XSpace[1],YSpace[0],YSpace[1]); Boid
b=boids[0]; Boid c=boids[1]; b.addBoid(c); std::cout<<&b<<' '<<&c<<'\n';
    std::set<Boid*> neighbors;
    neighbors.insert(&c);
    std::pair k=alignment(b,neighbors);
}
*/
/*
TEST_CASE("Testing isqrt")
{
    SUBCASE("gg"){
   }
    CHECK(isqrt(0) == 0);
    CHECK_THROWS
}
*/
