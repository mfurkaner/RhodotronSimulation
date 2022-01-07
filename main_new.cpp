#include "inc/simulation.h"
#include <chrono>

using namespace std::chrono;

uint64_t STEPS_TAKEN = 0;
double GUN_ACTIVE_TIME = 1; // ns
int NUM_OF_ELECTRONS = 100;
bool NOTIFICATIONS = false;
int MAX_THREAD_COUNT = 1;
bool MULTI_THREAD = false;
double dT = 0.001;     // ns
double dT_out = 0.01; // ns

int main(){
    auto start = high_resolution_clock::now();


    Gnuplot gp;
    gp.setRange(-1.5,1.5,-1.5,1.5);
    gp.enableMinorTics();
    gp.setCbRange(0, 1.5);
    gp.setCbTic(0.1);
    gp.setRatio(1);
    gp.addCommand("set isosamples 500,500");
    gp.addCommand("set cblabel \"Energy(MeV)\" offset 1,0,0");
    gp.addCommand("set palette rgb 33,13,10");
    gp.addCommand("set terminal gif animate delay 4");
    gp.addCommand("set output \"out.gif\"");
    std::string plotCommand = "do for [i=1:450] {plot \"xy/rf.txt\" every ::(i*916 - 915)::(i*916) using 5:7:($13/15):($15/15) title \"RF Field\" with vectors lc 6 head filled,";
    plotCommand += "\"xy/magnet.txt\" u 1:2 title \"magnets\" ls 5 lc 4 ps 0.2, ";                    // 4=sari
    plotCommand +=  "\"xy/path.txt\" every ::i::i u 15:17:12 title \"electron\" ls 7 ps 1.5 palette}";
    gp.setPlotCommand(plotCommand);


    DataStorage mag("xy/magnet.txt");
    DataStorage rf("xy/rf.txt");
    DataStorage path("xy/path.txt");
    rf.open();
    mag.open();
    path.open();

    vector3d magnet_position1(R2 + 0.35, 0, 0);
    magnet_position1.rotate(vector3d(0,0,1), -5);
    vector3d magnet_position2(R2 + 0.39144, 0, 0);
    magnet_position2.rotate(vector3d(0,0,1), 165);
    vector3d magnet_position3(R2 + 0.4, 0, 0);
    magnet_position3.rotate(vector3d(0,0,1), -25);

    Magnet m1(-0.029796384, 0.15, magnet_position1);
    Magnet m2(-0.039, 0.15, magnet_position2);
    Magnet m3(-0.054, 0.15, magnet_position3);
    MagneticField B;
    B.addMagnet(m1);
    B.addMagnet(m2);
    B.addMagnet(m3);
    B.log(mag);

    Simulator simulation(15);
    simulation.addMagnet(m1);
    simulation.addMagnet(m2);
    simulation.addMagnet(m3);
    simulation.setNumberofElectrons(100);
    //simulation.enableMultiThreading(5);
    simulation.run(path, rf);
    
    rf.close();
    mag.close();
    path.close();
    gp.executeCommands();
    gp.plot();
    gp.waitUntilDone();
    

    /*
    vector3d magnet_position(R2 + 0.35, 0, 0);
    magnet_position.rotate(vector3d(0,0,1), -5);
    Magnet m(-0.13, 1, magnet_position);
    cout << setprecision(6) << m.getOptimalB(0.45, -0.1, -0.01, 0.00001) << endl;
    */

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Simulation finished in : " << duration.count() << " us     ( "<<duration.count()/1000000.0 << " s )" << endl << endl;

    return 0;
}