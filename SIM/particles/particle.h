#ifndef PARTICLE_H
#define PARTICLE_H

#include "../basic/vector.h"
#include "../basic/consts.h"
#include "../basic/datastorage.h"
#include <vector>

enum ParticleType : int8_t {
    electron = 1,

    generic = -1,
    genericCharged = -2
};

struct ParticleLog{
    double time;
    double energy;
    vector3d pos;
    vector3d vel;
};

std::ostream& operator<<(std::ostream& stream, ParticleLog& log);

#pragma region PARTICLE

class Particle{
public:
    Particle(vector3d pos0, vector3d vel0, double mass, double time_created = 0, ParticleType type = generic);
    Particle(vector3d pos0, vector3d dir, double Ein, double mass, double time_created = 0, ParticleType type = generic);
    ~Particle();

    void move(double dt);
    void move(const vector3d& acc, double dt);
    void move(const vector3d& acc, const vector3d& jerk, double dt);

    void accelerate(const vector3d& acc, double dt);
    void accelerate(const vector3d& acc, const vector3d& jerk, double dt);

    inline double get_vel() const {
        return c*sqrt(Et*Et - _E_rest*_E_rest)/Et;
    }

    inline double beta(){
        return vel.magnitude()* _inv_c;
    }
    inline double beta2(){
        return vel.magnitude2() * _inv_c_sq;
    }
    inline double gamma(){
        return 1.0/(sqrt(1 - beta2()));
    }
    inline double gamma_inv(){
        return sqrt(1 - beta2());
    }
    inline void SetLogSize(size_t size){
        log.reserve(size);
    }
    inline void SetEin(double Ein){
        Et = Ein + _E_rest;
        vel = vel.direction()*get_vel();
    }

    void Log(DataStorage& path);
    void PrintInfo();
    void SaveInfo(double time);
    std::vector<ParticleLog>& GetLog(); 

    vector3d pos;
    vector3d vel;
    double Et;
    double mass;
    uint64_t ID(){return _id;}
    const double CreationTime(){return _creation_time;}
    const ParticleType type;
protected:
    double _E_rest;
    uint64_t _id;
    const double _creation_time;

    std::vector<ParticleLog> log;
};
#pragma endregion PARTICLE


#pragma region CHARGED PARTICLE
class ChargedParticle : public Particle{
public:
    ChargedParticle(vector3d pos0, vector3d vel0, double mass, double charge, 
        double time_created = 0, ParticleType type = genericCharged);

    ChargedParticle(vector3d pos0, vector3d dir, double Ein, double mass, 
        double charge, double time_created = 0, ParticleType type = genericCharged);
    ~ChargedParticle();
    

    const double q;
};
#pragma endregion CHARGED PARTICLE

#pragma region ELECTRON
class Electron : public ChargedParticle{
public:
    Electron(vector3d pos0, vector3d dir, double Ein, double fire_time);
    Electron(vector3d pos0, vector3d vel, double fire_time);


};


#endif