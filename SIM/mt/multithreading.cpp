#include "multithreadengine.h"


extern mutex mutex_lock;

void threadLoop(ThreadArguments thread_arguments){
    bool end = false;
    uint64_t count = 0;
    /*
    //thread_arguments.run_authorization->unlock();
    std::shared_lock<shared_mutex> lock(*thread_arguments.run_authorization, defer_lock);
    while (!end){

        //pthread_mutex_lock(thread_arguments.run_authorization.get());
        if (lock.try_lock()){
            if( *thread_arguments.end ){
                end = true;
            }
            else if ( *thread_arguments.run )
            {
                //thread_arguments.run_authorization->lock();
                thread_arguments.i_args.E = thread_arguments.E;
                thread_arguments.i_args.B = thread_arguments.B;
                interactForSingleThread(thread_arguments.i_args);
                // run for once
                *thread_arguments.run = false;
            }
            lock.unlock();
            //thread_arguments.run_authorization->unlock();
        }
        std::this_thread::yield();    
    }*/
    double sim_time = 0, end_time = 50;
    thread_arguments.i_args.E = thread_arguments.E;
    thread_arguments.i_args.B = thread_arguments.B;
    while(sim_time <= end_time){
        thread_arguments.E->update(sim_time);
        
        interactForSingleThread(thread_arguments.i_args);
        // save electron info here
        sim_time+= 0.0001;
    }
}

void interactForSingleThread(InteractArguments& interact_arguments){
    auto e_list = interact_arguments.e_list.get();
    auto e_size = e_list->size();
    //auto E = interact_arguments.E.get();
    //auto B = interact_arguments.B.get();

    for(int i = 0; i < e_size; i++){
        interact_arguments.e_list->at(i)->
            interactRK_ActorE(*interact_arguments.E, *interact_arguments.B, interact_arguments.time_interval);//, 
              //  interact_arguments.E_mutex, interact_arguments.B_mutex);
    }
}


void MultiThreadEngine::setupPool(vector<shared_ptr<vector<shared_ptr<Electron2D>>>>& e_list, CoaxialRFField& RF, MagneticField& B, double time_interval){
    threads.reserve(thread_count);
    thread_authorization_locks.reserve(thread_count);
    thread_run = new bool[thread_count]; 
    thread_end = new bool[thread_count]; 
    for(int i = 0; i < thread_count && i == threads.size(); i++){
        thread_authorization_locks.push_back(make_shared<shared_mutex>());
        thread_authorization_exc_locks.push_back(unique_lock<shared_mutex>(*thread_authorization_locks[i], defer_lock));
        /*
        if ( pthread_mutex_init(thread_authorization_locks[i].get(), NULL) != 0 ){
            perror("what is happening ? "); errno;
        }*/
        
        thread_run[i] = false;
        thread_end[i] = false;
        InteractArguments iargs;
        iargs.e_list = e_list[i];
        iargs.time_interval = time_interval;
        // Is this even a good idea?
        ThreadArguments thread_arguments(i, thread_authorization_locks[i], &thread_run[i], &thread_end[i], iargs);
        thread_arguments.E = RF.child(i);
        thread_arguments.B = B.child(i);
        threads.push_back(thread(threadLoop, thread_arguments));
        //this_thread::sleep_for(chrono::milliseconds(30));
        //thread_authorization_exc_locks[i].unlock();
    }
}

void MultiThreadEngine::runPool(double time){
    for(int i = 0; i < threads.size(); i++){
        //pthread_mutex_lock(thread_authorization_locks[i].get());
        thread_authorization_exc_locks[i].lock();
        thread_run[i] = true;
        //pthread_mutex_unlock(thread_authorization_locks[i].get());
        thread_authorization_exc_locks[i].unlock();
    }
}

void MultiThreadEngine::waitPool(){
    // Make sure the threads are finished
    for (int i = 0; i < thread_count; i++){
        //thread_authorization_exc_locks[i].lock();
        if ( thread_run[i]){
            i--;
            std::this_thread::yield();
        }
        //thread_authorization_exc_locks[i].unlock();
    }
}

void MultiThreadEngine::joinPool(){
    for (int i = 0; i < thread_count; i++){
        //pthread_mutex_lock(thread_authorization_locks[i].get());
        //thread_authorization_locks[i]->lock();
        thread_end[i] = true;
        //pthread_mutex_unlock(thread_authorization_locks[i].get());
        //thread_authorization_locks[i]->unlock();
    }

    for (int i = 0; i < threads.size();i++){
        threads[i].join();
    }

    delete [] thread_run;
    delete [] thread_end;
}

/*
void MultiThreadEngine::doWork(std::vector<std::vector<shared_ptr<Electron2D>>>& bunch, CoaxialRFField& RF, MagneticField& B, double time, double time_interval){
    for(int i = 0; i < bunch.size() ; i++){
        if ( bunch[i].size() == 0)
            continue;
        InteractArguments i_args;
        i_args.time_interval = time_interval;
        i_args.index = i;
        i_args.E = RF.Copy();
        i_args.B = B.LightWeightCopy();
        i_args.b = bunch[i];
        threads.push_back(thread(interactForSingleThread, i_args));
    }
}*/