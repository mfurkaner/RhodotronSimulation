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
    DataStorage* ds;
};

void* interactForSingleThread(void* interact_arguments);

class MultiThreadEngine{
    std::vector<pthread_t> threads;
    std::vector<void*> status;
    unsigned int thread_count;

    public:
    MultiThreadEngine(){}
    MultiThreadEngine(unsigned int thread_count):thread_count(thread_count){
        for(int i = 0; i < thread_count ; i++ ){
            pthread_t t;
            threads.push_back(t);
        }
    }

    void doWork(std::vector<Bunch2D*> bunch, RFField& RF, MagneticField& B, double time, double time_interval, DataStorage& ds){
        InteractArguments i_args;
        i_args.E = &RF;
        i_args.B = &B;
        i_args.time = time;
        i_args.time_interval = time_interval;
        i_args.ds = &ds;
        for(int i = 0; i < thread_count ; i++){
            i_args.b = bunch[i];
            cout << "thread " << i << " made it to create.  ";
            pthread_create(&threads.at(i), NULL, interactForSingleThread, &i_args);
            cout << "create complete\n";
        }
    }

    void join(){
        for(int i = 0; i < thread_count ; i++){
            void* p;
            status.push_back(p);
            cout << "thread " << i << " made it to join.  ";
            pthread_join(threads.at(i), &status[i]);
            cout << "join complete\n";
        }
    }

};