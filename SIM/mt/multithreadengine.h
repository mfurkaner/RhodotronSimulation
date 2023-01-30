#include <thread>         
#include <vector>   
#include <list>     
#include <array> 
#include <semaphore>
#include <mutex>
#include <pthread.h>
#include <shared_mutex>
#include <condition_variable>

#include "../particles/gun.h"
#include "../fields/fields.h"
#include "../gnuplot/gnuplot.h"

#ifndef MTENGINE_H
#define MTENGINE_H


struct InteractArguments{
    shared_ptr<vector<shared_ptr<Electron2D>>> e_list;
    CoaxialRFField* E;
    MagneticField* B;
    double time_interval;
};

struct ThreadArguments{
    shared_ptr<shared_mutex> run_authorization;

    //double time_interval;
    //double end_time;
    //double start_time;
 
    CoaxialRFField* E;
    MagneticField* B;
    int index;
    InteractArguments i_args;

    ThreadArguments(int id, shared_ptr<shared_mutex> authorization, 
    bool* _run, bool* _end, InteractArguments _i_args)
    :   run(_run), end(_end){
        index = id;
        run_authorization = authorization;
        i_args = _i_args;
    }
};

void interactForSingleThread(InteractArguments& interact_arguments);

void threadLoop(ThreadArguments thread_arguments);

class MultiThreadEngine{
    std::vector<thread> threads = {};
    std::vector<shared_ptr<shared_mutex>> thread_authorization_locks;
    std::vector<unique_lock<shared_mutex>> thread_authorization_exc_locks;
    std::condition_variable_any waiter;
    bool* thread_run;
    bool* thread_end;
    unsigned int thread_count = 0;

    public:
    MultiThreadEngine() {}
    MultiThreadEngine(unsigned int thread_count):thread_count(thread_count){}

    void setThreadCount(unsigned int _thread_count){thread_count = _thread_count;}

    void setupPool(vector<shared_ptr<vector<shared_ptr<Electron2D>>>>& e_list, CoaxialRFField& RF, MagneticField& B, double time_interval);

    void doWork(vector<shared_ptr<vector<shared_ptr<Electron2D>>>>& bunch, CoaxialRFField& RF, MagneticField& B, double time, double time_interval);

    // Run but promise for not changing the data you provided at setupPool().
    void runPool(double time);

    void waitPool();
    void joinPool();

    void join(){
        while(threads.size()){
            if ( threads.back().joinable()){
                threads.back().join();
                threads.pop_back();
            }
        }
    }

};
#endif