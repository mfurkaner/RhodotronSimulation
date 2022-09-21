#ifndef SIMULATION_H
    #include "inc/simulation.h"
#endif
#include "inc/gui/signal.h"

#include <chrono>
#include <thread>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>


//#define DEBUG


int getrusage(int who, struct rusage *usage);
void plot(Configuration& config);

using namespace std::chrono;

struct simtime{
    double start_time;
    double* simulation_time;
    double end_time;
    uint8_t* state_ptr;
    char* pipe_name;
};

mutex mutex_lock;
mutex state_lock;
uint8_t state = 0x0;
int _fd;

void* UIThreadWork(void* simtime_struct);

int main(int argc, char** argv) {

    pthread_t notifier;
    simtime notifierarg;

    if ( argc > 1  && strcmp(argv[1],"-fd") == 0 ) {
        std::cout << argv[1] << " " << argv[2] << " recieved" << std::endl;
        notifierarg.pipe_name = argv[2];
        _fd = open(argv[2], O_WRONLY | O_CREAT | O_APPEND);
    }
    auto start = high_resolution_clock::now();
    
    Configuration config("config.ini");
    config.getConfiguration();
    config.print();
    RhodotronSimulator rhodotron(config);


    notifierarg.start_time = config.getSTime();
    notifierarg.end_time = config.getETime();
    notifierarg.simulation_time = rhodotron.getTimePtr();
    notifierarg.state_ptr = &state;

    rhodotron.openLogs(); 
    pthread_create(&notifier, NULL, UIThreadWork, &notifierarg);

    state_lock.lock();
    state |= SIM_RUNNING;
    state_lock.unlock();

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

    if ( argc > 0  && strcmp(argv[1],"-fd") == 0 ){
        state_lock.lock();
        state &= ~SIM_RUNNING;
        write(_fd, &state, SIGNAL_SIZE);
        close(_fd);
        state_lock.unlock();
    }

    #ifdef DEBUG
        cout << "Simulation finished in : " << sim_time.count() << " us     ( "<<sim_time.count()/1000000.0 << " s )" << endl;
        cout << "Rendering finished in : " << render_time.count() << " us     ( "<<render_time.count()/1000000.0 << " s )" << endl;
    #endif

    return 0;
}

void* UIThreadWork(void* arg){

    int UI_WORK_PIECE = SIM_WORK_MASK;
    #ifdef DEBUG
        UI_WORK_PIECE = 50;
    #endif

    auto start = high_resolution_clock::now();

    simtime sim = *(simtime*)arg;
    double piece = (sim.end_time - sim.start_time)/UI_WORK_PIECE;

    //int _fd = open(sim.pipe_name, O_WRONLY | O_APPEND | O_CREAT);

    #ifdef DEBUG
        std::string sim_running_msg = "...Simulation is running...";
        for(int i = 0; i < 26 - sim_running_msg.size()/2 ; i++){
            cout << " ";
        }
        cout << sim_running_msg <<"\n";
    #endif

    mutex_lock.lock();
    double simtime = *(sim.simulation_time);
    mutex_lock.unlock();

    #ifdef DEBUG
        cout << "V";
        for(int i = 0; i < 51; i++){
            cout << "_";
        }
        cout << "V\n[" << flush;
    #endif

    int count = 0;
    while(simtime < sim.end_time || count < UI_WORK_PIECE){
        if( simtime > count * piece ){
            #ifdef DEBUG
                cout << "#" << flush;
            #endif
            count++;
            state_lock.lock();
            *sim.state_ptr = (*sim.state_ptr & ~SIM_WORK_MASK) | (count & SIM_WORK_MASK);
            write(_fd, sim.state_ptr, SIGNAL_SIZE);
            state_lock.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        mutex_lock.lock();
        simtime = *(sim.simulation_time);
        mutex_lock.unlock();

    }
    #ifdef DEBUG
        cout << "#]\n\n" << flush;
        cout << "     ...Simulation is finished successfully...\n\n" << flush;
    #endif

    state_lock.lock();
    *sim.state_ptr |= SIM_RENDERING;
    write(_fd, sim.state_ptr, SIGNAL_SIZE);
    state_lock.unlock();

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