#include "simulation/simulation.h"
#include "gnuplot/sim_renderer.h"
#include "../GUI/comm/signal.h"

#include <chrono>
#include <thread>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>


//#define DEBUG

RhodotronSimulator* sim_ptr;

void signal_handler(int signum){

    if(signum == SIGINT){
        std::cerr << "\nSIGINT received.\nStopping simulation..." << std::endl;
        if(sim_ptr != nullptr){
            sim_ptr->stop();
        }
        
        exit(signum);
    }


}


using namespace std::chrono;

bool isService = false;
bool renderSet = false;

int main(int argc, char** argv) {
    bool isService = false;

    Configuration config("config.ini");
    config.getConfiguration();
    RhodotronSimulator rhodotron(config);
    sim_ptr = &rhodotron;
    signal(SIGINT, signal_handler);

    // Is the simulation a service of GUI?
    if ( argc > 1  && strcmp(argv[1],"-fd") == 0 ) {
        rhodotron.DeclareService(argv[2]);
        isService = true;
    }else if(argc > 1 && strcmp(argv[1],"-r") == 0 ){
        renderSet = true;
    }
    auto start = high_resolution_clock::now();

    if ( ! isService ) config.print();
 
    rhodotron.openLogs(); 


    rhodotron.StartUIHandler();
    auto run_start = high_resolution_clock::now();
    rhodotron.run();
    auto run_stop = high_resolution_clock::now();
    rhodotron.logPaths();
    rhodotron.closeLogs();
    
    rhodotron.StopUIHandler();

    if ( ! isService ) {

        auto run_time = duration_cast<microseconds>(run_stop - run_start);
        auto sim_stop = high_resolution_clock::now();
        auto sim_time = duration_cast<microseconds>(sim_stop - start);

        cout << "Run finished in : " << run_time.count() << " us    ( " << run_time.count()/1000000.0 << " s )" << endl;
        cout << "Simulation finished in : " << sim_time.count() << " us     ( "<<sim_time.count()/1000000.0 << " s )" << endl;

        if (renderSet) {
            SimRenderer renderer;
            renderer.Render(config);
            auto render_stop = high_resolution_clock::now();
            auto render_time = duration_cast<microseconds>(render_stop - sim_stop);
            cout << "Rendering finished in : " << render_time.count() << " us     ( "<<render_time.count()/1000000.0 << " s )" << endl;
        }
    }

    return 0;
}
