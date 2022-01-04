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
    
    cout << rho_to_magnetic_field(0.029796384, 0.45) << endl;

    Gnuplot gp;
    gp.setRange(-1.5,1.5,-1.5,1.5);
    gp.setCbRange(0.04, 1.5);
    gp.setRatio(1);
    gp.addCommand("set palette rgb 33,13,10");
    gp.addCommand("set terminal gif animate delay 4");
    gp.addCommand("set output \"out.gif\"");
    gp.setPlotCommand("do for [i=1:450] {plot \"xy/engine.txt\" every ::(i*100-1)::(i*100) u 25:27:21 title \"electron\" ls 7 ps 1.5 palette, \"xy/magnet.txt\" u 1:2 title \"fields\" ls 7 ps 0.1}");
    DataStorage mag("xy/magnet.txt");
    DataStorage path("xy/engine.txt");
    mag.open();
    path.open();

    vector3d magnet_position1(R2 + 0.35, 0, 0);
    magnet_position1.rotate(vector3d(0,0,1), -5);
    vector3d magnet_position2(R2 + 0.39144, 0, 0);
    magnet_position2.rotate(vector3d(0,0,1), 165);
    vector3d magnet_position3(R2 + 0.4, 0, 0);
    magnet_position3.rotate(vector3d(0,0,1), -25);

    Magnet m1(-0.029796384, 0.2, magnet_position1);
    Magnet m2(-0.039, 0.20, magnet_position2);
    Magnet m3(-0.054, 0.20, magnet_position3);
    MagneticField B;
    B.addMagnet(m1);
    B.addMagnet(m2);
    B.addMagnet(m3);

    for(double x = -2; x <= 2 ; x+=0.02){
        for(double y = -2; y <= 2; y+=0.02){
            vector3d v(x,y,0);
            if ( B.getField(v).Z() != 0 || v.magnitude() <= R2 && v.magnitude() > R1){
                mag << x << "  " << y << "\n";
            }
        }
    }
    gp.executeCommands();
    gp.plot();
    Simulator simulation(15);
    simulation.addMagnet(m1);
    simulation.addMagnet(m2);
    simulation.addMagnet(m3);
    simulation.run(path, gp);
    
    
    mag.close();
    path.close();
    

   /*
    vector3d magnet_position1(R2 + 0.39144, 0, 0);
    magnet_position1.rotate(vector3d(0,0,1), -5);
    Magnet m1(-0.13, 1, magnet_position1);
    cout << setprecision(6) << m1.getOptimalB(0.823083, -0.4, -0.05, 0.00001) << endl;
*/
    return 0;
}