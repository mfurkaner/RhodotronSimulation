#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#define RHODO_H

const double c = 2.99792458e8; //m/s
const double E0=0.511;  // MeV
const double ns=1E-9; //ns conversion

//const double deg_to_rad = 0.01745;
extern int STEPS_TAKEN;
extern double GUN_ACTIVE_TIME; // ns

const double emass = 9.10938356e-31;    // kg
const double echarge = 1.60217662e-19;  //coulomb
const double eQMratio = -1.75882e11;    // C/kg

#define deg_to_rad  0.01745

#define Emax 0.960000 // MVolts/m
#define freq 107.5    // MHz
#define Ein 40.0      // KeV
#define dT 0.001      // ns
#define R1 0.188241   // m
#define R2 0.752967   // m
#define SimuTime 10   // ns
#define Emax_pos 200  // mm (Emax radial position)

#define MAGNET_ROTATION 5.0 //degrees
#define MAGNET_ROTATION_R MAGNET_ROTATION*deg_to_rad

#define dT_out 0.01    // ns
#define dD 0.001       // m
#define PHASE_SWEEP 15 // degrees

#define NUM_OF_ELECTRONS    50


#define TIME_BETWEEN_ELECTRONS GUN_ACTIVE_TIME/(NUM_OF_ELECTRONS - 1)

double Eradial(double r, double time, double phase);

double vel_to_dist(double vel, double t);

double dist_to_time(double dist, double vel);

int phase_opt(double Lout1, double Lout2, bool detail);
int phase_opt(double Lout1, double Lout2, double Lout3, bool detail);
vector<double> mag_opt(double RFphase, int magcount);

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
    Bunch( ){RFphase_bunch=0;}
    Bunch( double d ){RFphase_bunch=d;} 
    Electron e[NUM_OF_ELECTRONS];
    int e_count = NUM_OF_ELECTRONS;
    double initial_length_ns = GUN_ACTIVE_TIME;
    double ns_between = TIME_BETWEEN_ELECTRONS;
    int index_fastest = 0;
    int pass_count = 0;
    vector < max_energy_rms_pair > emax_rms;

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