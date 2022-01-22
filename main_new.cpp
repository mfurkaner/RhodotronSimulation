#ifndef SIMULATION_H
    #include "inc/simulation.h"
#endif

#include <chrono>
#include <thread>
#include <sys/time.h>
#include <sys/resource.h>
int getrusage(int who, struct rusage *usage);
void plot(double, uint64_t, bool);

using namespace std::chrono;

struct simtime{
    double start_time;
    double* simulation_time;
    double end_time;
};

pthread_mutex_t lck;

void* UIThreadWork(void* arg){
    for(int i = 0; i < 13; i++){
        cout << " ";
    }
    cout << "...Simulation is running...\nV";
    for(int i = 0; i < 50; i++){
        cout << " ";
    }
    cout << "V\n[" << flush;
    simtime sim = *(simtime*)arg;
    double piece = (sim.end_time - sim.start_time)/50;
    pthread_mutex_lock(&lck);
    double simtime = *(sim.simulation_time);
    pthread_mutex_unlock(&lck);
    int count = 0;
    while(simtime < sim.end_time){
        int a = simtime / piece ;
        if( a > count ){
            cout << "#" << flush;
            count++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        pthread_mutex_lock(&lck);
        simtime = *(sim.simulation_time);
        pthread_mutex_unlock(&lck);
    }
    cout << "#]\n\n" << flush;
    cout << "     ...Simulation is finished successfully...\n\n" << flush;
    return NULL;
}

int main(){
    auto start = high_resolution_clock::now();
    
    Configuration config("config.in");
    config.getConfiguration();
    config.print();
    RhodotronSimulator rhodotron(config);

    pthread_t notifier;
    simtime notifierarg;
    notifierarg.start_time = config.getSTime();
    notifierarg.end_time = config.getETime();
    notifierarg.simulation_time = rhodotron.getTimePtr();
    pthread_mutex_init(&lck, NULL);
    pthread_create(&notifier, NULL, UIThreadWork, &notifierarg);

    rhodotron.openLogs();
    rhodotron.run();
    rhodotron.logPaths();
    rhodotron.closeLogs();
    void*__nullable * a;
    pthread_join(notifier, a);
    pthread_mutex_destroy(&lck);

    plot(config.getETime(), config.getNumOfE(), true);

    

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Simulation finished in : " << duration.count() << " us     ( "<<duration.count()/1000000.0 << " s )" << endl;

    return 0;
}


void plot(double simulation_time, uint64_t NUM_OF_ELECTRONS, bool isThereMagnet){
    Gnuplot gp;
    gp.setRange(-1.5,1.5,-1.5,1.5);
    gp.enableMinorTics();
    gp.setCbRange(0, 1.6);
    gp.setCbTic(0.1);
    gp.setRatio(1);
    gp.addCommand("set isosamples 500,500");
    gp.addCommand("set cblabel \"Energy(MeV)\" offset 1,0,0");
    gp.addCommand("set palette rgb 33,13,10");
    gp.addCommand("set terminal gif animate delay 3");
    gp.addCommand("set output \"out.gif\"");
    gp.addCommand("set key top left");
    
    std::string plotCommand = "do for [i=1:" + to_string(simulation_time*10 - 1) + "] {plot \"xy/rf.txt\" every ::(i*916 - 915)::(i*916) using 5:7:($13/15):($15/15) title \"RF Field\" with vectors lc 6 head filled,";
    plotCommand += (isThereMagnet) ? "\"xy/magnet.txt\" u 1:2 title \"magnets\" ls 5 lc 4 ps 0.2, " : "";                    // 4=sari
    plotCommand +=  "\"xy/paths/e" + to_string(1) +".txt\" every ::i::i u 3:4:2 title \"bunch\" ls 7 ps 0.5 palette, ";
    for( int j = 2 ; j <= NUM_OF_ELECTRONS ; j++){
        plotCommand +=  "\"xy/paths/e" + to_string(j) +".txt\" every ::i::i u 3:4:2 notitle ls 7 ps 0.5 palette, ";
    }
    plotCommand += "}";
    gp.setPlotCommand(plotCommand);

    gp.executeCommands();
    gp.plot();
    gp.waitUntilDone();
}