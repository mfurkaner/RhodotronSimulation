#include <thread>         
#include <vector>         
#ifndef RHODO2D_H
    #include "rhodo2d.h"
#endif
#ifndef FIELDS_H
    #include "fields.h"
#endif
#ifndef GNUPLOT_H
    #include "gnuplot.h"
#endif

#define MTENGINE_H

struct InteractArguments{
    Bunch2D* b;
    RFField* E;
    MagneticField* B;
    double time;
    double time_interval;
};

void interactForSingleThread(InteractArguments interact_arguments);
class MultiThreadEngine{
    std::vector<thread> threads = {};
    unsigned int thread_count;

    public:
    MultiThreadEngine(){}
    MultiThreadEngine(unsigned int thread_count):thread_count(thread_count){}

    void doWork(std::vector<Bunch2D*> bunch, RFField& RF, MagneticField& B, double time, double time_interval){
        InteractArguments i_args;
        i_args.E = &RF;
        i_args.B = &B;
        i_args.time = time;
        i_args.time_interval = time_interval;
        for(int i = 0; i < thread_count ; i++){
            i_args.b = bunch[i];
            threads.push_back(thread(interactForSingleThread, i_args));
        }
    }

    void join(){
        while(threads.size()){
            threads.back().join();
            threads.pop_back();
        }
    }

};