#ifndef BUNCH_H
#define BUNCH_H

#include "../basic/consts.h"
#include "../basic/vector.h"
#include "../fields/fields.h"
#include "../interact/interactor.h"
#include "particle.h"

using namespace std;


typedef pair<double, double> max_energy_rms_pair;

class Bunch{
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
    vector<Bunch> subBunchs;
    vector<shared_ptr<Electron>> e;
    /*
    Bunch(unsigned int num_of_electrons, double Ein, vector3d gunpos, vector3d gundir, double gun_ns){
        E_in = Ein;
        e_count = num_of_electrons;
        initial_length_ns = gun_ns;
        _LEGACY_gun_pos = gunpos;
        _LEGACY_gun_dir = gundir;
        for(int i = 0 ; i < num_of_electrons ; i++){
            e.push_back(make_shared<Electron>(Electron(gunpos, gundir, Ein, )));
            e.back()->SetLogSize(1000);
        }
        _LEGACY_ns_between = initial_length_ns/(e_count - 1);
    }*/

    Bunch(){
        e_count = 0;
    }

    void saveInfo(double time){
        for(auto i : e){
            i->SaveInfo(time);
        }
    }

    Electron& getFastest();

    void AddElectron(double Ein, const vector3d& gunpos, const vector3d& gundir, double fire_time){
        e.push_back(make_shared<Electron>(gunpos, gundir, Ein, fire_time));
        e.back()->SetLogSize(1000);
        e_count = e.size();
    }

    shared_ptr<Electron> AddElectronGiveAddress(double Ein, const vector3d& gunpos, const vector3d& gundir, double fire_time){
        AddElectron(Ein, gunpos, gundir, fire_time);
        return e.back();
    }

    void setEntryTime(double entry_time){this->entry_time = entry_time;}
    void setEin(double E_in){ this->E_in = E_in ; for (int i = 0; i < e.size() ; i++){ e[i]->SetEin(E_in);}}
    void setNSLen(double len){ initial_length_ns = len; _LEGACY_ns_between = initial_length_ns/(e_count - 1);}
    double getEin(){ return E_in;}
    void interact(RFField& E, MagneticField& B, const double time, double time_interval);
    //void divide(unsigned int num);
    //void concat();
    //Bunch& subBunch(unsigned int index);
    //vector<Bunch*> subBunchPtr();
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
            e[i]->Log(pathStorage[i]);
            pathStorage[i].close();
        }
    }
};

#endif