#include "consts.h"

using namespace std;
#define RHODO_H


const double c = 2.99792458e8; //m/s
const double E0=0.511;  // MeV
const double ns=1E-9; //ns conversion


//const double deg_to_rad = 0.01745;
extern uint64_t STEPS_TAKEN;
extern double GUN_ACTIVE_TIME; // ns
extern int NUM_OF_ELECTRONS;
extern bool NOTIFICATIONS;
extern int MAX_THREAD_COUNT;
extern bool MULTI_THREAD;

const double _emass_ = 9.10938356e-31;    // kg
const double _echarge_ = 1.60217662e-19;  //coulomb
const double eQMratio = -1.75882e11;    // C/kg

#define deg_to_rad  0.01745
#define US_FOR_A_STEP 0.06467

#define Emax 1.78000 // MVolts/m
#define freq 107.38    // MHz
#define Ein 40.0      // KeV
extern double dT;     // ns
extern double dT_out; // ns
#define R1 0.184   // m
#define R2 0.736   // m
#define Emax_pos 200  // mm (Emax radial position)

#define MAGNET_ROTATION 5.0 //degrees
#define MAGNET_ROTATION_R MAGNET_ROTATION*deg_to_rad

#define dD 0.001       // m
#define PHASE_SWEEP 15 // degrees
#define MAGNET_TIME_SWEEP_MIN 2.0 // ns
#define MAGNET_TIME_SWEEP_MAX 9.0 // ns


double Eradial(double r, double time, double phase);

double vel_to_dist(double vel, double t);

double dist_to_time(double dist, double vel);

int phase_opt(double Lout1, double Lout2, bool detail);
int phase_opt(double Lout1, double Lout2, double Lout3, bool detail);
vector<double> mag_opt(double RFphase, int magcount, bool detail);

pair<double, double> distout_to_Lrho_pair( double dist_out );

void displayHelp();




typedef pair<double, double> giris_cikis_tpair;

class Electron{
public:
    double Et = E0 + Ein/1000;
    double r_pos = -R2;
    double RFphase = 0;
    vector< double > enerjiler;
    vector< giris_cikis_tpair > t_giris_cikis;

    double get_vel();

    double get_travel_time(double dist);

    void print_electron_info();

    void e_gecis(double &t);
};


typedef pair<double, double> max_energy_rms_pair;

class Bunch{
public:
    vector<Electron> e;
    int e_count = NUM_OF_ELECTRONS;
    double initial_length_ns = GUN_ACTIVE_TIME;
    double ns_between = GUN_ACTIVE_TIME/(NUM_OF_ELECTRONS - 1);
    int index_fastest = 0;
    int pass_count = 0;
    vector < max_energy_rms_pair > emax_rms;

    Bunch( ){
        RFphase_bunch=0;
        for(int i = 0 ; i < NUM_OF_ELECTRONS ; i++){
            e.push_back(Electron());
        }        
    }
    Bunch( double d ){
        RFphase_bunch=d;
        for(int i = 0 ; i < NUM_OF_ELECTRONS ; i++){
            e.push_back(Electron());
        }
    } 

    double E_ave();
    double E_rms();
    void reset_pos();
    void print_bunch_info();
    void print_summary();
    void bunch_gecis_t(double &t_delay_of_max);
    void bunch_gecis_d(double dist_out);
private:
    double RFphase_bunch;
    void bunch_ilk_gecis(double &t);
    void bunch_nth_gecis_t(double t_delay_of_max);
    void bunch_nth_gecis_d(double dist_out);
};
