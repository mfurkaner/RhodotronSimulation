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
#include "../interact/interactor.h"


#ifndef MTENGINE_H
#define MTENGINE_H


struct InteractArguments{
    shared_ptr<vector<shared_ptr<Electron>>> e_list;
    shared_ptr<CoaxialRFField> E;
    shared_ptr<MagneticField> B;
    double time_interval;
    double time;
};

struct ThreadArguments{
    shared_ptr<bool> terminate;
    shared_ptr<bool> wait;
    shared_ptr<mutex> parent_notifier_mutex;
    shared_ptr<double> current_thread_time;

    double time_interval;
    double end_time;
    double start_time;
    double time_between_each_fire;
    double time_of_first_fire;
 
    shared_ptr<CoaxialRFField> E;
    shared_ptr<MagneticField> B;
    Gun* gun;
    int index;
    InteractArguments i_args;

    ThreadArguments(int id,  double _time_interval, double _start_time, double _end_time, Gun* gun, 
        shared_ptr<CoaxialRFField> _E, shared_ptr<MagneticField> _B, shared_ptr<vector<shared_ptr<Electron>>> e_list, 
        double _time_of_first_fire, double _time_between_each_fire){

        index = id;
        time_interval = _time_interval;
        start_time = _start_time;
        end_time = _end_time;
        E = _E;
        B = _B;
        i_args.e_list = e_list;
        i_args.E = _E;
        i_args.B = _B;
        i_args.time_interval = _time_interval;
        time_between_each_fire = _time_between_each_fire;
        time_of_first_fire = _time_of_first_fire;
    }

};

void e_EM_interaction_forSingleThread(InteractArguments& interact_arguments);
void saveElectronInfoForSingleThread(InteractArguments& interact_arguments);

void e_EM_interactorThreadLoop(ThreadArguments thread_arguments);

void e_e_interactor_thread();

class MultiThreadEngine{
    std::vector<thread> threads = {};
    std::vector<shared_ptr<bool>> child_terminate;
    std::vector<shared_ptr<mutex>> child_notifier_mutexes;
    std::vector<shared_ptr<double>> child_times; 
    unsigned int thread_count = 0;

    void _Handle_ee_Interactions();

    public:
    MultiThreadEngine() {}
    MultiThreadEngine(unsigned int thread_count):thread_count(thread_count){}

    void setThreadCount(unsigned int _thread_count){thread_count = _thread_count;}

    void setupPool( double _time_interval, double _start_time, double _end_time, Gun& gun, 
        CoaxialRFField& RF, MagneticField& B, vector<shared_ptr<vector<shared_ptr<Electron>>>>& e_list);


    void stopPool();


    void join();

    double getAverageTime();

};
#endif