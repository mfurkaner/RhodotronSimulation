#include "inc/simulation.h"
#include <chrono>
#include <sys/time.h>
#include <sys/resource.h>

int getrusage(int who, struct rusage *usage);
void plot(double);

using namespace std::chrono;

uint64_t STEPS_TAKEN = 0;
double GUN_ACTIVE_TIME = 1; // ns
int NUM_OF_ELECTRONS = 100;
bool NOTIFICATIONS = false;
int MAX_THREAD_COUNT = 1;
bool MULTI_THREAD = false;
double dT = 0.01;     // ns
double dT_out = 0.01; // ns

int main(){
    auto start = high_resolution_clock::now();
    int simulation_time = 5;

    /*

    vector3d magnet_position1(R2 + 0.35, 0, 0);
    magnet_position1.rotate(vector3d(0,0,1), -5);
    vector3d magnet_position2(R2 + 0.39144, 0, 0);
    magnet_position2.rotate(vector3d(0,0,1), 165);
    vector3d magnet_position3(R2 + 0.4, 0, 0);
    magnet_position3.rotate(vector3d(0,0,1), -25);

    Magnet m1(-0.029796384, 0.15, magnet_position1);
    Magnet m2(-0.039, 0.15, magnet_position2);
    Magnet m3(-0.065, 0.15, magnet_position3);
    magnet_position3.rotate(vector3d(0,0,1), +170);
    Magnet m4(-0.065, 0.15, magnet_position3);
    magnet_position3.rotate(vector3d(0,0,1), +170);
    Magnet m5(-0.065, 0.15, magnet_position3);

    Simulator simulation(0);
    simulation.addMagnet(m1);
    simulation.addMagnet(m2);
    simulation.addMagnet(m3);
    simulation.addMagnet(m4);
    simulation.addMagnet(m5);
    simulation.setNumberofElectrons(NUM_OF_ELECTRONS);
    simulation.setEndTime(simulation_time);
    //simulation.enableMultiThreading(5);

    simulation.openLogs();
    simulation.run();
    simulation.logPaths();
    simulation.closeLogs();



    Gnuplot gp;
    gp.setRange(-1.5,1.5,-1.5,1.5);
    gp.enableMinorTics();
    gp.setCbRange(0, 3);
    gp.setCbTic(0.1);
    gp.setRatio(1);
    gp.addCommand("set isosamples 500,500");
    gp.addCommand("set cblabel \"Energy(MeV)\" offset 1,0,0");
    gp.addCommand("set palette rgb 33,13,10");
    gp.addCommand("set terminal gif animate delay 3");
    gp.addCommand("set output \"out.gif\"");
    gp.addCommand("set key top left");
    
    std::string plotCommand = "do for [i=1:" + to_string(simulation_time*10) + "] {plot \"xy/rf.txt\" every ::(i*916 - 915)::(i*916) using 5:7:($13/15):($15/15) title \"RF Field\" with vectors lc 6 head filled,";
    plotCommand += "\"xy/magnet.txt\" u 1:2 title \"magnets\" ls 5 lc 4 ps 0.2, ";                    // 4=sari
    plotCommand +=  "\"xy/paths/e" + to_string(1) +".txt\" every ::i::i u 3:4:2 title \"bunch\" ls 7 ps 0.5 palette, ";
    for( int j = 2 ; j <= NUM_OF_ELECTRONS ; j++){
        plotCommand +=  "\"xy/paths/e" + to_string(j) +".txt\" every ::i::i u 3:4:2 notitle ls 7 ps 0.5 palette, ";
    }
    plotCommand += "}";
    gp.setPlotCommand(plotCommand);

    gp.executeCommands();
    gp.plot();
    gp.waitUntilDone();
    

    /*
    vector3d magnet_position(R2 + 0.35, 0, 0);
    magnet_position.rotate(vector3d(0,0,1), -5);
    Magnet m(-0.13, 1, magnet_position);
    cout << setprecision(6) << m.getOptimalB(0.45, -0.1, -0.01, 0.00001) << endl;*/

    Simulator simulation(0);
    simulation.setEin(1);
    simulation.setNumberofElectrons(NUM_OF_ELECTRONS);
    simulation.setEmax(0);
    simulation.setEndTime(simulation_time);

    vector3d v(0.05,0,0);
    Magnet m(0.1,10,v);
    simulation.addMagnet(m);
    simulation.openLogs();
    simulation.runBonly();
    simulation.logPaths();
    simulation.closeLogs();

    plot(simulation_time);

    

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Simulation finished in : " << duration.count() << " us     ( "<<duration.count()/1000000.0 << " s )" << endl << "Energy of the electron : " << simulation.getElectronWithMaxEnergy().Et - E0 << endl;
    rusage ru;
    getrusage( RUSAGE_SELF, &ru);
    cout << "Size of simulation : " << ru.ru_ixrss << endl;
    return 0;
}


void plot(double simulation_time){
    Gnuplot gp;
    gp.setRange(-0.2,0.2,-0.1,0.3);
    gp.enableMinorTics();
    gp.setCbRange(1, 4);
    gp.setCbTic(0.1);
    gp.setRatio(1);
    gp.addCommand("set isosamples 500,500");
    gp.addCommand("set cblabel \"Energy(MeV)\" offset 1,0,0");
    gp.addCommand("set palette rgb 33,13,10");
    gp.addCommand("set terminal gif animate delay 5");
    gp.addCommand("set output \"out.gif\"");
    gp.addCommand("set key top left");
    
    std::string plotCommand = "do for [i=1:" + to_string(simulation_time*10) + "] {plot \"xy/rf.txt\" every ::(i*916 - 915)::(i*916) using 5:7:($13/15):($15/15) title \"RF Field\" with vectors lc 6 head filled,";
    plotCommand += "\"xy/magnet.txt\" u 1:2 title \"magnets\" ls 5 lc 4 ps 0.2, ";                    // 4=sari
    plotCommand +=  "\"xy/paths/e" + to_string(1) +".txt\" every ::i::i u 3:4:2 title \"bunch\" ls 7 ps 0.5 palette, ";
    for( int j = 2 ; j <= NUM_OF_ELECTRONS ; j++){
        plotCommand +=  "\"xy/paths/e" + to_string(j) +".txt\" every ::i::i u 3:4:2 notitle ls 7 ps 0.5 palette, ";
    }
    std::string plotCommandB = "do for [i=1:" + to_string(simulation_time*10) + "] {plot ";
    plotCommandB += "\"xy/magnet.txt\" u 1:2 title \"magnets\" ls 5 lc 4 ps 0.2, ";                    // 4=sari
    plotCommandB +=  "\"xy/paths/e" + to_string(1) +".txt\" every ::i::i u 3:4:2 title \"bunch\" ls 7 ps 0.5 palette, ";
    for( int j = 2 ; j <= NUM_OF_ELECTRONS ; j++){
        plotCommandB +=  "\"xy/paths/e" + to_string(j) +".txt\" every ::i::i u 3:4:2 notitle ls 7 ps 0.5 palette, ";
    }
    plotCommandB += "}";

    gp.setPlotCommand(plotCommandB);

    gp.executeCommands();
    gp.plot();
    gp.waitUntilDone();
}