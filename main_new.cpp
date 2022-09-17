#ifndef SIMULATION_H
    #include "inc/simulation.h"
#endif

#include <chrono>
#include <thread>
#include <sys/time.h>
#include <sys/resource.h>
int getrusage(int who, struct rusage *usage);
void plot(Configuration& config);

using namespace std::chrono;

struct simtime{
    double start_time;
    double* simulation_time;
    double end_time;
};

mutex mutex_lock;

void* UIThreadWork(void* simtime_struct);

int main(){
    auto start = high_resolution_clock::now();
    
    Configuration config("config.ini");
    config.getConfiguration();
    config.print();
    RhodotronSimulator rhodotron(config);

    pthread_t notifier;
    simtime notifierarg;
    notifierarg.start_time = config.getSTime();
    notifierarg.end_time = config.getETime();
    notifierarg.simulation_time = rhodotron.getTimePtr();

    rhodotron.openLogs(); 
    pthread_create(&notifier, NULL, UIThreadWork, &notifierarg);
    rhodotron.run();
    rhodotron.logPaths();
    rhodotron.closeLogs();
    void* a;
    pthread_join(notifier, &a);

    auto sim_stop = high_resolution_clock::now();
    auto sim_time = duration_cast<microseconds>(sim_stop - start);
    plot(config);
    auto render_stop = high_resolution_clock::now();
    auto render_time = duration_cast<microseconds>(render_stop - sim_stop);
    cout << "Simulation finished in : " << sim_time.count() << " us     ( "<<sim_time.count()/1000000.0 << " s )" << endl;
    cout << "Rendering finished in : " << render_time.count() << " us     ( "<<render_time.count()/1000000.0 << " s )" << endl;
    return 0;
}

void* UIThreadWork(void* arg){
    auto start = high_resolution_clock::now();
    std::string sim_running_msg = "...Simulation is running...";

    simtime sim = *(simtime*)arg;
    double piece = (sim.end_time - sim.start_time)/50;

    for(int i = 0; i < 26 - sim_running_msg.size()/2 ; i++){
        cout << " ";
    }
    cout << sim_running_msg <<"\n";

    mutex_lock.lock();
    double simtime = *(sim.simulation_time);
    mutex_lock.unlock();

    cout << "V";
    for(int i = 0; i < 51; i++){
        cout << "_";
    }
    cout << "V\n[" << flush;
    int count = 0;
    while(simtime < sim.end_time || count < 50){
        if( simtime > count * piece ){
            cout << "#" << flush;
            count++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        mutex_lock.lock();
        simtime = *(sim.simulation_time);
        mutex_lock.unlock();
    }
    cout << "#]\n\n" << flush;
    cout << "     ...Simulation is finished successfully...\n\n" << flush;
    return NULL;
}


void plot(Configuration& config){
    Gnuplot gp;
    gp.setRange(-1.5,1.5,-1.5,1.5);
    gp.enableMinorTics();
    gp.setCbRange(0.49, config.getTargetEnergy());
    gp.setCbTic(0.1);
    gp.setRatio(1);
    gp.addCommand("set isosamples 500,500");
    gp.addCommand("set cblabel \"Energy(MeV)\" offset 1,0,0");
    gp.addCommand("set palette rgb 33,13,10");
    gp.addCommand("set terminal gif animate delay 3");
    gp.addCommand("set output \""+ config.getOutput() + "\"");
    gp.addCommand("set key top left");
    
    std::string plotCommand = "do for [i=1:" + to_string(config.getETime()*10 - 1) + "] {plot"/* \"xy/rf.txt\" every ::(i*916 - 915)::(i*916) using 5:7:($13/15):($15/15) title \"RF Field\" with vectors lc 6 head filled,"*/;
    plotCommand += (/*config.areThereMagnets()*/ 0 ) ? "\"xy/magnet.txt\" u 1:2 title \"magnets\" ls 5 lc 4 ps 0.2, " : "";                    // 4=sari
    plotCommand +=  "\"xy/paths/e" + to_string(1) +".txt\" every ::i::i u 3:4:2 title \"bunch\" ls 7 ps 0.5 palette, ";
    for( int j = 2 ; j <= config.getNumOfE()*config.getNumOfB() ; j++){
        plotCommand +=  "\"xy/paths/e" + to_string(j) +".txt\" every ::i::i u 3:4:2 notitle ls 7 ps 0.5 palette, ";
    }
    plotCommand += "}";
    gp.setPlotCommand(plotCommand);

    gp.executeCommands();
    gp.plot();
    gp.waitUntilDone();
}