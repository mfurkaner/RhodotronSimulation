#include "../basic/consts.h"
#include "../basic/vector.h"
#include "../fields/fields.h"
#include "../gnuplot/gnuplot.h"

using namespace std;


#ifndef ELECTRON_H
#define ELECTRON_H
#define RUNGE_KUTTA
//#define LEAP_FROG

double Eradial(double r, double time, double phase);


typedef pair<double, double> giris_cikis_tpair;

struct ElectronLog{
    double time;
    vector3d pos;
    vector3d vel;
    double energy;
};


std::ostream& operator<<(std::ostream& stream, ElectronLog& log);
class Electron2D{
    std::vector<ElectronLog> log;
public:
    uint64_t index;
    double Et = E0;
    vector3d pos;
    vector3d vel;
    vector< double > enerjiler;
    vector< giris_cikis_tpair > t_giris_cikis;
    bool isinside = true;
    Electron2D(){}
    Electron2D(double Ein, vector3d position, vector3d direction){
        Et += Ein;
        t_giris_cikis.push_back(giris_cikis_tpair(0,0));
        pos = position;
        vel = direction*get_vel();
    }
    
    double get_vel();
    void setEin(double E_in){this->Et = E_in + E0; vel.setX(get_vel());};
    void print_electron_info();
    void move(double dt);
    void move(const vector3d& acc, double dt);
    void move(const vector3d& acc, const vector3d& jerk, double dt);
    void accelerate(const vector3d& acc, double dt);
    void accelerate(const vector3d& acc, const vector3d& jerk, double dt);
    
    void interactLF(RFField& E, MagneticField& B, double time_interval);
    void interactRK(RFField& E, MagneticField& B, double time_interval);

    void saveInfo(double t){
        ElectronLog info;
        info.time = t;
        info.pos = pos;
        info.vel = vel;
        info.energy = Et - E0;
        log.push_back(info);
    }
    void setLogSize(size_t size){
        log.reserve(size);
    }

    void loge(DataStorage& path){
        for(int i = 0; i < log.size() ; i++){
            path << log[i] << (i + 1 == log.size() ? "" : "\n" ) ;
        }
    }

    vector<ElectronLog>& getLog(){
        return log;
    }

    double beta(){
        return vel.magnitude()* _inv_c;
    }
    double beta2(){
        return vel.magnitude2() * _inv_c_sq;
    }
    double gamma(){
        return 1.0/(sqrt(1 - beta2()));
    }
    double gamma_inv(){
        return sqrt(1 - beta2());
    }
};



#endif