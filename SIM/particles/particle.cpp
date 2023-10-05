#include "particle.h"


    std::ostream& operator<<(std::ostream& stream, ParticleLog& log){
        stream << log.time << "," << log.energy << "," << log.pos << "," << log.vel;
        return stream;
    }

    Particle::Particle(vector3d pos0, vector3d vel0, double mass, double time_created, ParticleType type) 
        : pos(pos0), vel(vel0), mass(mass), type(type), _creation_time(time_created){
        _E_rest = mass * c * c * joule_to_mev;

        Et = gamma() * _E_rest;
    }

    Particle::Particle(vector3d pos0, vector3d dir, double Ein, double mass, double time_created, ParticleType type) 
            : pos(pos0), mass(mass), type(type), _creation_time(time_created){
        _E_rest = mass * c * c * joule_to_mev;

        Et = _E_rest + Ein;
        vel = dir*get_vel();
    }

    Particle::~Particle(){}

    void Particle::move(double dt){
        pos += vel*(dt*ns);
    }
    void Particle::move(const vector3d& acc, double dt){
        pos += vel*(dt*ns) + acc*(dt*ns)*(dt*ns)/2;
    }
    void Particle::move(const vector3d& acc, const vector3d& jerk, double dt){
        pos += vel*(dt*ns) + acc*(dt*ns)*(dt*ns)/2 + jerk*(dt*ns*dt*ns*dt*ns)/6 ;
    }

    void Particle::accelerate(const vector3d& acc, double dt){
        vel += acc*(dt*ns);  
        Et = gamma()*E0;          
    }
    void Particle::accelerate(const vector3d& acc, const vector3d& jerk, double dt){
        vel += acc*(dt*ns) + jerk*(dt*ns*dt*ns)/2;  
        Et = gamma()*E0;     
    }

    void Particle::Log(DataStorage& path){
        for(int i = 0; i < log.size() ; i++){
            path << log[i] << (i + 1 == log.size() ? "" : "\n" ) ;
        }
    }
    void Particle::PrintInfo(){

    }

    void Particle::SaveInfo(double time){
        ParticleLog info;
        info.time = time;
        info.pos = pos;
        info.vel = vel;
        info.energy = Et - _E_rest;
        log.push_back(info);
    }

    std::vector<ParticleLog>& Particle::GetLog(){
        return log;
    }



    ChargedParticle::ChargedParticle(vector3d pos0, vector3d vel0, double mass, double charge, 
        double time_created, ParticleType type) 
            : Particle(pos0, vel0, mass, time_created, type), q(charge){

    }

    ChargedParticle::ChargedParticle(vector3d pos0, vector3d dir, double Ein, double mass, 
        double charge, double time_created, ParticleType type) 
            : Particle(pos0, dir, Ein, mass, time_created, type), q(charge) {

    }

    ChargedParticle::~ChargedParticle(){}


    Electron::Electron(vector3d pos0, vector3d dir, double Ein, double fire_time) 
        : ChargedParticle(pos0, dir, Ein, _emass_, _echarge_, fire_time, electron){

    }

    Electron::Electron(vector3d pos0, vector3d vel, double fire_time) 
        : ChargedParticle(pos0, vel, _emass_, _echarge_, fire_time, electron){

    }