#ifndef BOID
#define BOID


#include <stdexcept>
#include <set>
#include <cmath> 
#include <cassert>
#include <utility>

#include "config.hpp"
extern const Config& cfg;

class Boid
{
private:
    double _x;
    double _y;
    double _vx;
    double _vy;
    std::set<Boid*> neighbors;
    //I use raw pointers, I have to be careful not to leave dangling pointers and assure that the objects 
    //they're pointing to do not go out of scope before we end up dealing with these pointers.
public:
    explicit Boid(double x, double y, double vx, double vy): _x{x}, _y{y}, _vx{vx}, _vy{vy}
    {
        if(std::sqrt(vx*vx+vy*vy)>cfg.getVMax()){
            throw std::invalid_argument("Velocity exceeds the maximum allowed value.");
        }
        if (x>=cfg.getXSpace()[1] ||y>=cfg.getYSpace()[1] ||x<=cfg.getXSpace()[0] || y<=cfg.getYSpace()[0]){
            throw std::invalid_argument("Boid initalized out of available space.");
        }
    }
    //to make sure we have read only pointers in neighbour vector.
    double get_x() const {return _x;}
    double get_vx() const {return _vx;}
    double get_y() const {return _y;}
    double get_vy() const {return _vy;}
    void addBoid(Boid const& boid){
        //out of space?
        //assert (!(boid.get_x()>=XSpace[1] ||boid.get_y()>=YSpace[1] ||boid.get_x()<=XSpace[0] || boid.get_y()<=YSpace[0]));

        //I'm using a set since the insertion is at worst log(n), I left as the needed comparison for the construction of the implicit balanced tree
        //the default one based on the memory address which should work great since we will define our boid objects once for all at the beginning.
        neighbors.insert(const_cast<Boid*>(&boid));
    }
    void removeBoid(Boid const& boid){
        neighbors.erase(const_cast<Boid*>(&boid));
    }

    double distance (Boid const& other) const;

    bool inSight(Boid const& other)  const{
        return distance(other)<cfg.getD();
    }

    void updateVelocity();
    void updatePosition();
};

std::pair<double,double> separation(Boid const& current, std::set<Boid*> const& neighbor);
std::pair<double,double> alignment(Boid const& current, std::set<Boid*> const& neighbor);
std::pair<double,double> cohesion(Boid const& current, std::set<Boid*> const& neighbor);

#endif
