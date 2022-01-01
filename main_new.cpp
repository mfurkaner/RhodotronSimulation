#include "inc/simulation.h"

uint64_t STEPS_TAKEN = 0;
double GUN_ACTIVE_TIME = 1; // ns
int NUM_OF_ELECTRONS = 1;
bool NOTIFICATIONS = false;
int MAX_THREAD_COUNT = 1;
bool MULTI_THREAD = false;
double dT = 0.001;     // ns
double dT_out = 0.01; // ns

int main(){
    double max = 0;
    double maxB;
    for (double i = 0.1; i < 3; i+=0.001){
        Simulator simulation(0);
        Electron2D e;
        simulation.addMagnet(i, 0.3 , vector3d(1.1995, -0.2115, 0));
        simulation.run();
        e = simulation.getElectronWithMaxEnergy();
        /*
        if(e.t_giris_cikis.size() > 1 && e.t_giris_cikis[1].first > 5 && e.t_giris_cikis[1].first < 13){
            cout << "When the magnetic field is at " << i << " T , electron enters at " << e.t_giris_cikis[1].first << " ns , finishing with the energy  : " << e.Et - E0 << endl;
        }*/
        if(e.t_giris_cikis.size() <= 1){
            cout << "When the magnetic field is at " << i << " T,  electron cannot enter." << endl;
        }
    }
    
    
    return 0;
}