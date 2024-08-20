#include "boid.hpp"
#include <iostream>

//It would have been good to construct a new modDouble variable maybe
double mod(double num, double modulo){
    if(!cfg.isTorusSpace()){
        return num;
    }
    else{

        if (std::abs(num)>std::abs(std::abs(num)-modulo) ){
            if (num>=0){
            return num-modulo;
            }
            else{
                return num+modulo;
            }
        }
        else{
            return num;
        }
    }
}
std::pair<double,double> separation(Boid const& current, std::set<Boid*> const& neighbors){
    double separationX = 0.0;
    double separationY = 0.0;

    for (const auto& neighbor : neighbors) {
        if (neighbor == &current) continue;

        double distanceX = mod(neighbor->get_x() - current.get_x(),cfg.getXSpace()[1]-cfg.getXSpace()[0]);
        double distanceY = mod(neighbor->get_y() - current.get_y(),cfg.getYSpace()[1]-cfg.getYSpace()[0]);
        double distance = current.distance(*neighbor);

        if (distance < cfg.getDS()) {
            separationX -= cfg.getS() * distanceX;
            separationY -= cfg.getS() * distanceY;
        }
    }

    return {separationX, separationY};
}

std::pair<double, double> alignment(Boid const& current, std::set<Boid*> const& neighbors) {
    //std::cout<<&current<<' '<<*(neighbors.begin());
    double avgVx = 0.0;
    double avgVy = 0.0;
    int count = 0;

    for (const auto& neighbor : neighbors) {
        //As for now I update neighbors with lazyUpdateNeighbors it shouldn't contain itself
        //in the neighbors but to make sure even for later
        if (neighbor == &current) continue;

        avgVx += neighbor->get_vx();
        avgVy += neighbor->get_vy();
        count++;
    }

    if (count > 0) {
        avgVx /= count;
        avgVy /= count;
    }

    double alignmentX = cfg.getA() * (avgVx - current.get_vx());
    double alignmentY = cfg.getA() * (avgVy - current.get_vy());

    return {alignmentX, alignmentY};
}

std::pair<double, double> cohesion(Boid const& current, std::set<Boid*> const& neighbors) {
    double centerX = 0.0;
    double centerY = 0.0;
    int count = 0;

    // Calcola il centro di massa (media delle posizioni dei boids vicini)
    for (const auto& neighbor : neighbors) {
        if (neighbor == &current) continue;

        centerX += mod(neighbor->get_x()-current.get_x(),cfg.getXSpace()[1]-cfg.getXSpace()[0]);
        centerY +=  mod(neighbor->get_y()-current.get_y(),cfg.getYSpace()[1]-cfg.getYSpace()[0]);
        count++;
    }

    if (count > 0) {
        centerX /= count;
        centerY /= count;
    }

    // Calcola la velocità di coesione
    double cohesionX = cfg.getC() * centerX; 
    double cohesionY = cfg.getC() * centerY;

    return {cohesionX, cohesionY};
}