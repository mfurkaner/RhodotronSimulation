#include "../basic/consts.h"
#include "../basic/vector.h"
#include "../fields/fields.h"
#include "../gnuplot/gnuplot.h"
#include "electron.h"

using namespace std;


#ifndef BUNCH_H
#define BUNCH_H


typedef pair<double, double> max_energy_rms_pair;

class Bunch2D{
private:
    int e_count = 0;
    double initial_length_ns = 1;
    double _LEGACY_ns_between = 0.1;
    int index_fastest = 0;
    double max_energy = 0;
    double entry_time = 0;
    double E_in = 0.04;
    vector3d _LEGACY_gun_pos;
    vector3d _LEGACY_gun_dir;
public:
    vector<Bunch2D> subBunchs;
    vector<Electron2D> e;
    Bunch2D(unsigned int num_of_electrons, double Ein, vector3d gunpos, vector3d gundir, double gun_ns){
        E_in = Ein;
        e_count = num_of_electrons;
        initial_length_ns = gun_ns;
        _LEGACY_gun_pos = gunpos;
        _LEGACY_gun_dir = gundir;
        for(int i = 0 ; i < num_of_electrons ; i++){
            e.push_back(Electron2D(Ein, gunpos, gundir));
            e.back().setLogSize(1000);
        }
        _LEGACY_ns_between = initial_length_ns/(e_count - 1);
    }

    Bunch2D(){
        e_count = 0;
    }

    /*Bunch2D(){
        Bunch2D(1, 0.4, vector3d(-0.753,0,0), vector3d(1,0,0), 0);
    }*/

    void saveInfo(double time){
        for(Electron2D& i : e){
            i.saveInfo(time);
        }
    }

    Electron2D& getFastest();

    void AddElectron(double Ein, const vector3d& gunpos, const vector3d& gundir){
        e.push_back(Electron2D(Ein, gunpos, gundir));
        e.back().setLogSize(1000);
        e_count = e.size();
    }

    void setEntryTime(double entry_time){this->entry_time = entry_time;}
    void setEin(double E_in){ this->E_in = E_in ; for (int i = 0; i < e.size() ; i++){ e[i].setEin(E_in);}}
    void setNSLen(double len){ initial_length_ns = len; _LEGACY_ns_between = initial_length_ns/(e_count - 1);}
    double getEin(){ return E_in;}
    void interact(RFField& E, MagneticField& B, double time_interval);
    void divide(unsigned int num);
    void concat();
    Bunch2D& subBunch(unsigned int index);
    vector<Bunch2D*> subBunchPtr();
    double E_ave();
    double E_rms();
    void reset();
    void print_bunch_info();
    void print_summary();

    uint32_t get_e_count(){return e_count;}

    void logPaths(vector<DataStorage>& pathStorage, std::string header){
        for(int i = 0; i < e.size() && i < pathStorage.size(); i++){
            pathStorage[i].open();
            pathStorage[i] << header << "\n";
            e[i].loge(pathStorage[i]);
            pathStorage[i].close();
        }
    }
};

#endif