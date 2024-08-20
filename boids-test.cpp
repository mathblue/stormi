#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "boid.hpp"
#include "helper.hpp"
#include "rules.hpp"
#include "parameters.hpp"

TEST_CASE("Testing Boid class"){
    CHECK_NOTHROW(Boid {10.0,10.0,10.0,10.0});
    CHECK_THROWS(Boid {200.0,10.0,10000.0,10.0});
}
/*
TEST_CASE("Testing rules"){
    std::vector<Boid> boids=generateBoids(N_BOIDS,XSpace[0],XSpace[1],YSpace[0],YSpace[1]);
    Boid b=boids[0];
    Boid c=boids[1];
    b.addBoid(c);
    std::cout<<&b<<' '<<&c<<'\n';
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
