
#include <math.h>
#include <vector>
using namespace std;

const double c = 2.99792458e8; //m/s
const double E0=0.511;  // MeV
const double ns=1E-9; //ns conversion

const double deg_to_rad = 0.01745;

const double emass = 9.10938356e-31;  // kg
const double echarge = 1.60217662e-19; //coulomb
const double eQMratio = -1.75882e11; // C/kg


#define Emax 0.960000 // MVolts/m
#define freq 107.5    // MHz
#define Ein 40.0      // KeV
#define RFphase 0    // degree
#define dT 0.001        // ns
#define R1 0.1882     // m
#define R2 0.753      // m
#define SimuTime 10   // ns
#define Emax_pos 200 // mm (Emax radial position)

#define L1 0.4        // m (distance to first magnet)
#define Rho1 0.23     // m (radius of the first magnet)

#define L2 0.4        // m (distance to second magnet)
#define Rho2 0.23     // m (radius of the second magnet)

#define dT_out 0.01    // ns

#define NUM_OF_ELECTRONS    50
#define GUN_ACTIVE_TIME     1.0 // ns

#define TIME_BETWEEN_ELECTRONS GUN_ACTIVE_TIME/(NUM_OF_ELECTRONS - 1)

typedef pair<double, double> giris_cikis_tpair;

class Electron{
public:
    double Et = E0 + Ein/1000;
    double r_pos = -R2;
    vector< giris_cikis_tpair > t_giris_cikis;
    double get_vel(){
        return c*sqrt(Et*Et-E0*E0)/Et;
    }
};

class Bunch{
public:
    Electron e[NUM_OF_ELECTRONS];
    int e_count = NUM_OF_ELECTRONS;
    double initial_length_ns = GUN_ACTIVE_TIME;
    double ns_between = TIME_BETWEEN_ELECTRONS;
};