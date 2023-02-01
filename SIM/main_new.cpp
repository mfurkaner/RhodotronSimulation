#include "simulation/simulation.h"
#include "../GUI/comm/signal.h"

#include <chrono>
#include <thread>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>


//#define DEBUG

void plot(Configuration& config);

using namespace std::chrono;

struct simtime{
    double start_time;
    double* simulation_time;
    double end_time;
    uint8_t* state_ptr;
    char* pipe_name;
    shared_ptr<mutex> ui_mutex;
};

mutex state_lock;
uint8_t state = 0x0;
int _fd;
bool isService = false;
bool plotSet = false;

void* UIThreadWork(void* simtime_struct);

int main(int argc, char** argv) {

    pthread_t notifier;
    simtime notifierarg;

    if ( argc > 1  && strcmp(argv[1],"-fd") == 0 ) {
        std::cout << argv[1] << " " << argv[2] << " recieved" << std::endl;
        notifierarg.pipe_name = argv[2];
        _fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        isService = true;
    }else if(argc > 1 && strcmp(argv[1],"-r") == 0 ){
        plotSet = true;
    }
    auto start = high_resolution_clock::now();
    
    Configuration config("config.ini");
    config.getConfiguration();
    if ( ! isService ) config.print();

    RhodotronSimulator rhodotron(config);

    notifierarg.start_time = config.getSTime();
    notifierarg.end_time = config.getETime();
    notifierarg.simulation_time = rhodotron.getTimePtr();
    notifierarg.ui_mutex = rhodotron.getUIMutex();
    notifierarg.state_ptr = &state;
    pthread_create(&notifier, NULL, UIThreadWork, &notifierarg);
 
    rhodotron.openLogs(); 

    state_lock.lock();
    state |= SIM_RUNNING;
    state_lock.unlock();

    auto run_start = high_resolution_clock::now();
    rhodotron.run();
    auto run_stop = high_resolution_clock::now();
    rhodotron.logPaths();
    rhodotron.closeLogs();
    
    void* a;
    pthread_join(notifier, &a);


    if ( isService ){
        //std::cout << "What are you doing here?" << std::endl;
        state_lock.lock();
        state &= ~SIM_RUNNING;
        write(_fd, &state, SIGNAL_SIZE);
        close(_fd);
        state_lock.unlock();
    }

    else {

        auto run_time = duration_cast<microseconds>(run_stop - run_start);
        auto sim_stop = high_resolution_clock::now();
        auto sim_time = duration_cast<microseconds>(sim_stop - start);

        if (plotSet) plot(config);

        auto render_stop = high_resolution_clock::now();
        auto render_time = duration_cast<microseconds>(render_stop - sim_stop);

        cout << "Run finished in : " << run_time.count() << " us    ( " << run_time.count()/1000000.0 << " s )" << endl;
        cout << "Simulation finished in : " << sim_time.count() << " us     ( "<<sim_time.count()/1000000.0 << " s )" << endl;
        cout << "Rendering finished in : " << render_time.count() << " us     ( "<<render_time.count()/1000000.0 << " s )" << endl;
    }

    return 0;
}

void* UIThreadWork(void* arg){

    int UI_WORK_PIECE = SIM_WORK_MASK;
    if ( !isService ) {
        UI_WORK_PIECE = 50;
    }


   //auto start = high_resolution_clock::now();

    simtime sim = *(simtime*)arg;
    double piece = (sim.end_time - sim.start_time)/UI_WORK_PIECE;

    //int _fd = open(sim.pipe_name, O_WRONLY | O_APPEND | O_CREAT);

    if ( !isService ) {
        std::string sim_running_msg = "...Simulation is running...";
        for(int i = 0; i < 26 - sim_running_msg.size()/2 ; i++){
            cout << " ";
        }
        cout << sim_running_msg <<"\n";
    }

    sim.ui_mutex->lock();
    double simtime = *(sim.simulation_time);
    sim.ui_mutex->unlock();

    if ( !isService ) {
        cout << "V";
        for(int i = 0; i < 51; i++){
            cout << "_";
        }
        cout << "V\n[" << flush;
    }

    int count = 0;
    while(simtime < sim.end_time || count < UI_WORK_PIECE){
        if( simtime > count * piece ){
            if ( !isService ) {
                cout << "#" << flush;
            }
            count++;
            state_lock.lock();
            *sim.state_ptr = (*sim.state_ptr & ~SIM_WORK_MASK) | (count & SIM_WORK_MASK);
            if (isService) write(_fd, sim.state_ptr, SIGNAL_SIZE);
            state_lock.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        sim.ui_mutex->lock();
        simtime = *(sim.simulation_time);
        sim.ui_mutex->unlock();

    }
    if ( !isService ) {
        cout << "#]\n\n" << flush;
        cout << "     ...Simulation is finished successfully...\n\n" << flush;
    }

    state_lock.lock();
    *sim.state_ptr |= SIM_RENDERING;
    if ( isService ) write(_fd, sim.state_ptr, SIGNAL_SIZE);
    state_lock.unlock();

    return NULL;
}


void plot(Configuration& config){
    Gnuplot gp;
    gp.setRange(-1.5,1.5,-1.5,1.5);
    gp.enableMinorTics();
    gp.setCbRange(config.getEin(), config.getTargetEnergy());
    gp.setCbTic(0.1);
    gp.setRatio(1);
    gp.addCommand("set isosamples 500,500");
    gp.addCommand("set cblabel \"Energy(MeV)\" offset 1,0,0");
    gp.addCommand("set palette rgb 33,13,10");
    gp.addCommand("set terminal gif animate delay 3");
    gp.addCommand("set output \""+ config.getOutput() + "\"");
    gp.addCommand("set key top left");
    
    std::string plotCommand = "do for [i=1:" + to_string(config.getETime()*10 - 1) + "] ";
    plotCommand += "{plot \"xy/rf.txt\" every ::(i*916 - 915)::(i*916) using 1:2:($3/15):($4/15) '%*lf,( %lf ; %lf ; %*lf ),( %lf ; %lf ; %*lf ),%lf' title \"RF Field\" with vectors lc 6 head filled,";
    plotCommand += (config.areThereMagnets() ) ? "\"xy/magnet.txt\" u 1:2 title \"magnets\" ls 5 lc 4 ps 0.2, " : "";                    // 4=sari
    for( int j = 1 ; j <= config.getNumOfB() ; j++){
        for ( int i = 1 ; i <= config.getNumOfE(); i++)
        plotCommand +=  "\"xy/paths/b" + to_string(j) + "_e" + to_string(i) +".dat\" every ::i::i u 2:3:1 '%*lf,%lf,( %lf ; %lf ; %*lf ),( %*lf ; %*lf ; %*lf )' " + (j==1 && i == 1 ? "title \"bunch\"" : "notitle") + " ls 7 ps 0.5 palette, ";
    }
    plotCommand += "}";
    gp.setPlotCommand(plotCommand);

    gp.executeCommands();
    gp.plot();
    gp.waitUntilDone();
}
