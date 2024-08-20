#include "boid.hpp"
#include "helper.hpp"
#include "BoidRenderer.hpp"
#include "config.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

void printInfo(std::vector<Boid> const& boids){
    std::cout<<'\n';
    std::pair Distance=averageDistanceAndStdDev(boids);
    std::cout<<"Avg. Distance: "<<Distance.first<<" std. dev.:"<<Distance.second<<'\n';

    auto [avgVelocity, stdDevVelocity] = averageVelocityAndStdDev(boids);
    std::cout << "Average Velocity: (" << avgVelocity.first << ", " << avgVelocity.second << ")" << std::endl;
    std::cout << "Standard Deviation of Velocity: (" << stdDevVelocity.first << ", " << stdDevVelocity.second << ")" << std::endl;
}
const Config& cfg = Config::getInstance();
int main(){
    // Load the configuration once
    Config& config = const_cast<Config&>(Config::getInstance());
    config.loadConfig("config.txt");
    
    BoidRenderer renderer(1500, 1000, "Boids Simulation");


    std::vector<Boid> boids=generateBoids(cfg.getNBoids(),cfg.getXSpace()[0],cfg.getXSpace()[1],
    cfg.getYSpace()[0],cfg.getYSpace()[1]);
    lazyUpdateNeighbors(boids);
    for (Boid b : boids){
        std::cout<<b.get_vx()<<' '<<b.get_vy()<<'\n';
    }
    for (double time=0;time<cfg.getTTot();time+=cfg.getDt()){
        renderer.processEvents();

        std::for_each(boids.begin(),boids.end(),[](Boid& b) {b.updateVelocity(); b.updatePosition();});

        lazyUpdateNeighbors(boids);

        renderer.render(boids);

        if (static_cast<int>(time/cfg.getDt())%static_cast<int>(10/cfg.getDt())==0){
            printInfo(boids);
        }

        if (!renderer.isOpen()) {
                break;
            }
        }

}