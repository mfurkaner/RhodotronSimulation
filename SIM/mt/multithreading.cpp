#include "multithreadengine.h"


extern mutex mutex_lock;

void e_EM_interactorThreadLoop(ThreadArguments thread_arguments){
    uint64_t count = 0;

    double sim_time = thread_arguments.start_time;

    while(sim_time < thread_arguments.end_time + thread_arguments.time_interval){
        if(*thread_arguments.wait) {std::this_thread::yield(); continue;}
        if(*thread_arguments.terminate) return;
        thread_arguments.E->update(sim_time);
        thread_arguments.i_args.time = sim_time;
        
        if ( count % (unsigned long)(0.1/thread_arguments.time_interval) == 0){
            saveElectronInfoForSingleThread(thread_arguments.i_args);
            // Notifiy the main thread
            if(thread_arguments.parent_notifier_mutex->try_lock()){
                *thread_arguments.current_thread_time = sim_time;
                thread_arguments.parent_notifier_mutex->unlock();
            }
        }

        e_EM_interaction_forSingleThread(thread_arguments.i_args);
        // save electron info here
        sim_time+= thread_arguments.time_interval;
        count++;
    }
    thread_arguments.parent_notifier_mutex->lock();
    *thread_arguments.current_thread_time = sim_time;
    thread_arguments.parent_notifier_mutex->unlock();
}

void e_EM_interaction_forSingleThread(InteractArguments& interact_arguments){
    auto e_list = interact_arguments.e_list.get();
    auto e_size = e_list->size();
    //auto E = interact_arguments.E.get();
    //auto B = interact_arguments.B.get();

    for(int i = 0; i < e_size; i++){
        if ((*e_list)[i]->CreationTime() < interact_arguments.time){
            vector3d acc = Interactor::q_EM_interaction_RK(*(*e_list)[i], *interact_arguments.E, 
                *interact_arguments.B, interact_arguments.time_interval);
            
            (*e_list)[i]->move(interact_arguments.time_interval/2);
            (*e_list)[i]->accelerate(acc, interact_arguments.time_interval);
            (*e_list)[i]->move(interact_arguments.time_interval/2);

            //(*e_list)[i]->interactRK_ActorE(*interact_arguments.E, *interact_arguments.B, interact_arguments.time_interval);
        }
    }
}

void saveElectronInfoForSingleThread(InteractArguments& interact_arguments){
    auto e_list = interact_arguments.e_list.get();
    auto e_size = e_list->size();
    //auto E = interact_arguments.E.get();
    //auto B = interact_arguments.B.get();

    for(int i = 0; i < e_size; i++){
        (*e_list)[i]->SaveInfo(interact_arguments.time);
    }
}

void MultiThreadEngine::setupPool( double _time_interval, double _start_time, double _end_time, Gun& gun, 
    CoaxialRFField& RF, MagneticField& B, vector<shared_ptr<vector<shared_ptr<Electron>>>>& e_list){
    threads.reserve(thread_count);

    for(int i = 0; i < thread_count && i == threads.size(); i++){
        child_terminate.push_back(make_shared<bool>(false));
        child_notifier_mutexes.push_back(make_shared<mutex>());
        child_times.push_back(make_shared<double>());
        // Is this even a good idea?
        auto _E = RF.Copy();
        auto _B = B.LightWeightCopy();
        double time_between_fires = thread_count*0.8/50;
        double first_fire_time = i*0.8/50;
        ThreadArguments thread_arguments(i, _time_interval, _start_time, _end_time, &gun, _E, _B, e_list[i], first_fire_time, time_between_fires);
        thread_arguments.terminate = child_terminate[i];
        thread_arguments.parent_notifier_mutex = child_notifier_mutexes[i];
        thread_arguments.current_thread_time = child_times[i];
        threads.push_back(thread(e_EM_interactorThreadLoop, thread_arguments));
    }
}

void MultiThreadEngine::stopPool(){
    for(int i = 0; i < child_terminate.size(); i++){
        *child_terminate[i] = true;
    }
    join();
}

void MultiThreadEngine::join(){
    while(threads.size()){
        if ( threads.back().joinable()){
            threads.back().join();
            threads.pop_back();
        }
    }
}

double MultiThreadEngine::getAverageTime(){
    double result = 0.0;
    for(int i = 0; i < threads.size(); i++){
        child_notifier_mutexes[i]->lock();
        result += *child_times[i];
        child_notifier_mutexes[i]->unlock();
    }
    return result/thread_count;
}