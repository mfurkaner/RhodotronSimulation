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

    Gnuplot gp;
    gp.setRange(-1.5,1.5,-1.5,1.5);
    gp.setCbRange(0.04, 1);
    gp.setRatio(1);
    gp.addCommand("set palette rgb 33,13,10");
    gp.addCommand("set terminal gif animate delay 4");
    gp.addCommand("set output \"out.gif\"");
    gp.setPlotCommand("do for [i=1:180] {plot \"xy/engine.txt\" every ::(i*100-1)::(i*100) u 25:27:21 title \"electron\" ls 7 ps 1.5 palette, \"xy/magnet.txt\" u 1:2 title \"fields\" ls 7 ps 0.1}");
    DataStorage mag("xy/magnet.txt");
    DataStorage path("xy/engine.txt");
    mag.open();
    path.open();
    /*
    double max = 0;
    double maxB;
    for (double i = 0.1; i < 3; i+=0.001){*/

        /*
        if(e.t_giris_cikis.size() > 1 && e.t_giris_cikis[1].first > 5 && e.t_giris_cikis[1].first < 13){
            cout << "When the magnetic field is at " << i << " T , electron enters at " << e.t_giris_cikis[1].first << " ns , finishing with the energy  : " << e.Et - E0 << endl;
        }*/
        /*
        if(e.t_giris_cikis.size() <= 1){
            cout << "When the magnetic field is at " << i << " T,  electron cannot enter." << endl;
        }*/
    //}
    vector3d magnet_position(R2 + 0.2574, 0, 0);
    magnet_position.rotate(vector3d(0,0,1), -5);
    MagneticField B;
    B.addMagnet(-0.14, 0.2 , magnet_position);

    for(double x = -2; x <= 2 ; x+=0.025){
        for(double y = -2; y <= 2; y+=0.025){
            vector3d v(x,y,0);
            if ( B.getField(v).Z() != 0 || v.magnitude() <= R2 && v.magnitude() > R1){
                mag << x << "  " << y << "\n";
            }
        }
    }
    gp.executeCommands();
    gp.plot();
    Simulator simulation(15);
    simulation.addMagnet(-0.14, 0.2 , magnet_position);
    simulation.run(path, gp);
    
    
    mag.close();
    path.close();
    
/*
    vector3d r2pos(R2, 0, 0);
    for(double i = 0; i <= 360 ; i+=0.01){
        cout << r2pos << endl;
        r2pos.rotate(vector3d(0,0,1), 0.01);
    }
*/
    
    return 0;
}