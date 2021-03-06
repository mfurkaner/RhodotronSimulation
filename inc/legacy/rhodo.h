#include "consts.h"

using namespace std;
#define RHODO_H



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