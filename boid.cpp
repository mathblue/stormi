#include "boid.hpp"


template <typename T,typename U>                                                   
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first+r.first,l.second+r.second};                                    
}

double Boid::distance (Boid const& other) const{
        double distance;
        if (cfg.isTorusSpace()){
        double dx = std::abs(_x - other.get_x());
        double dy = std::abs(_y - other.get_y());
        dx = std::min(dx, cfg.getXSpace()[1] - dx);
        dy = std::min(dy, cfg.getYSpace()[1] - dy);
        distance = std::sqrt(dx * dx + dy * dy);
        }
        else{
        double dx = std::abs(_x - other.get_x());
        double dy = std::abs(_y - other.get_y());
        distance = std::sqrt(dx * dx + dy * dy);   
        }
        return distance;       
    }

void Boid::updateVelocity(){
    std::pair<double,double> V_toAdd={0.0,0.0};
    V_toAdd=V_toAdd+separation(*this,neighbors);
    V_toAdd=V_toAdd+alignment(*this,neighbors);
    V_toAdd=V_toAdd+cohesion(*this,neighbors);

    _vx+=V_toAdd.first;
    _vy+=V_toAdd.second;

    double norm =std::sqrt(_vx*_vx+_vy*_vy);
    if (norm>cfg.getVMax()){
        _vx=_vx/norm*cfg.getVMax();
        _vy=_vy/norm*cfg.getVMax();
        
    }

}

void Boid::updatePosition(){
    _x+=cfg.getDt()*_vx;
    _y+=cfg.getDt()*_vy;

    if (cfg.isTorusSpace()){
    double Xdim=(cfg.getXSpace()[1]-cfg.getXSpace()[0]);
    double Ydim=(cfg.getYSpace()[1]-cfg.getYSpace()[0]);
    //apply modulo
    if (_x>cfg.getXSpace()[1]){
        _x=_x-Xdim;
    }
    if (_x<cfg.getXSpace()[0]){
        _x=_x+Xdim;
    }
    if (_y>cfg.getYSpace()[1]){
        _y=_y-Ydim;
    }
    if (_y<cfg.getYSpace()[0]){
        _y=_y+Ydim;
    }
    }
    else{
        if (_x>cfg.getXSpace()[1]){
            _vx-=cfg.getRepulsion()*(_x-cfg.getXSpace()[1]);
        }
        if (_x<cfg.getXSpace()[0]){
            _vx+=cfg.getRepulsion()*(cfg.getXSpace()[0]-_x);
        }
        if (_y>cfg.getYSpace()[1]){
            _vy-=cfg.getRepulsion()*(_y-cfg.getYSpace()[1]);
        }
        if (_y<cfg.getYSpace()[0]){
            _vy+=cfg.getRepulsion()*(cfg.getYSpace()[0]-_y);
        }
    }
}