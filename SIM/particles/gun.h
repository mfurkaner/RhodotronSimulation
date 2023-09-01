#include "../basic/consts.h"
#include "../basic/vector.h"
#include "../fields/fields.h"
#include "../gnuplot/gnuplot.h"
#include "bunch.h"
#include <memory>

using namespace std;


#ifndef GUN_H
#define GUN_H
#define RUNGE_KUTTA
//#define LEAP_FROG


class Gun{
    double Ein;
    double sEin;
    double gun_period;
    double gun_active_time;
    double ns_between_each_electron_fire;

    vector3d gunpos;
    vector3d gundir = vector3d(1,0,0);

    uint64_t bunch_count = 0;
    uint64_t e_per_bunch = 1;

    uint32_t _fired_bunch = 0;
    uint32_t _fired_e_in_current_bunch = 0;
    bool _firing = false;

    bool _mt_enabled = false;
    uint32_t _child_thread_count = 0;

    void setNSLen(double len){ for (int i = 0; i < bunchs.size() ; i++){  bunchs[i].setNSLen(len); }}
public:
    vector<Bunch> bunchs;

    mutex _gun_mutex; 
    vector<shared_ptr<vector<shared_ptr<Electron>>>> thread_bunchs;

    Gun();
    Gun(double Ein, double gun_active_time, double pulse_interval, vector3d gunpos);
    void interact(RFField& E, MagneticField& B, const double time, double time_interval);

    void fireIfActive(double time);
    void fireAllWithFireTimesMT();
    
    void addBunch(unsigned int num_of_electrons, double Ein);

    void setGunActiveTime(double gt);
    void setGunInterval(double guninterval);
    void setGunPos(vector3d gun_pos);
    void setGunDir(vector3d gun_dir);
    void setEin(double Ein) ;
    void setEinStd(double EinStd) ;
    void setNumberOfElectrons(uint64_t e_num);
    void setNumberOfBunchs(uint64_t b_num);
    void enableMT(uint32_t thread_count);

    double getGunAcvtiveTime(){return gun_active_time;}
    double getePerBunch() {return e_per_bunch;}
    
    void saveInfo(double time);
    void logPaths(vector<vector<DataStorage> >& pathsStorage, std::string pathsPath, std::string header);

};


#endif 