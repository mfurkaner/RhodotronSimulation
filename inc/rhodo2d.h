#ifndef CONSTS_H
    #include "consts.h"
#endif

#ifndef VECTOR_H
    #include "vector.h"
#endif

#ifndef FIELDS_H
    #include "fields.h"
#endif


using namespace std;
#define RHODO2D_H

double Eradial(double r, double time, double phase);


typedef pair<double, double> giris_cikis_tpair;

class Electron2D{
public:
    double Et = E0 + Ein/1000;
    vector3d pos;
    vector3d vel;
    double RFphase = 0;
    vector< double > enerjiler;
    vector< giris_cikis_tpair > t_giris_cikis;
    bool isinside = true;
    Electron2D(){
        t_giris_cikis.push_back(giris_cikis_tpair(0,0));
        pos.setX(-R2);
        vel.setX(get_vel());
    }
    double get_vel();

    double get_travel_time(double dist);

    void print_electron_info();
    void move(double dt);
    void accelerate(vector3d acc, double dt);
    void e_gecis(double &t);
};



typedef pair<double, double> max_energy_rms_pair;

class Bunch2D{
public:
    vector<Electron2D> e;
    int e_count = NUM_OF_ELECTRONS;
    double initial_length_ns = GUN_ACTIVE_TIME;
    double ns_between = GUN_ACTIVE_TIME/(NUM_OF_ELECTRONS - 1);
    double RFphase_bunch;
    int index_fastest = 0;
    double max_energy = 0;
    double entry_time = 0;
    Bunch2D( ){
        RFphase_bunch=0;
        for(int i = 0 ; i < NUM_OF_ELECTRONS ; i++){
            e.push_back(Electron2D());
        }        
    }
    Bunch2D( double d ){
        RFphase_bunch=d;
        for(int i = 0 ; i < NUM_OF_ELECTRONS ; i++){
            e.push_back(Electron2D());
        }
    } 

    void interact(RFField& E, MagneticField& B, double time, double time_interval);

    double E_ave();
    double E_rms();
    void reset();
    void print_bunch_info();
    void print_summary();
};