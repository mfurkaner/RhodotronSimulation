#include "ui_handler.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "../../GUI/comm/signal.h"


void UIThreadWork(UIThreadArgs args){

    int UI_WORK_PIECE = SIM_WORK_MASK;
    if ( !args.isService ) {
        UI_WORK_PIECE = 50;
    }


    double piece = (args.end_time - args.start_time)/UI_WORK_PIECE;

    //int _fd = open(sim.pipe_name, O_WRONLY | O_APPEND | O_CREAT);

    if ( !args.isService ) {
        std::string sim_running_msg = "...Simulation is running...";
        for(int i = 0; i < 26 - sim_running_msg.size()/2 ; i++){
            std::cout << " ";
        }
        std::cout << sim_running_msg <<"\n";
    }

    args.ui_mutex->lock();
    double simtime = *(args.simulation_time);
    args.ui_mutex->unlock();

    if ( !args.isService ) {
        std::cout << "V";
        for(int i = 0; i < 51; i++){
            std::cout << "_";
        }
        std::cout << "V\n[" << std::flush;
    }

    int count = 0;
    bool running = true;
    while(running && (simtime < args.end_time || count < UI_WORK_PIECE )){
        if( simtime > count * piece ){
            if ( !args.isService ) {
                std::cout << "#" << std::flush;
            }
            count++;
            args.state_mutex->lock();
            running = *args.state_ptr & SIM_RUNNING;
            *args.state_ptr = (*args.state_ptr & ~SIM_WORK_MASK) | (count & SIM_WORK_MASK);
            if (args.isService) write(args._fd, args.state_ptr, SIGNAL_SIZE);
            args.state_mutex->unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        args.ui_mutex->lock();
        simtime = *(args.simulation_time);
        args.ui_mutex->unlock();

    }
    if ( !args.isService ) {
        std::cout << "#]\n\n" << std::flush;
        std::cout << "     ...Simulation is finished successfully...\n\n" << std::flush;
    }

    args.state_mutex->lock();
    *args.state_ptr |= SIM_RENDERING;
    if ( args.isService ) write(args._fd, args.state_ptr, SIGNAL_SIZE);
    args.state_mutex->unlock();

}

void UIHandler::SetUIThreadArgs(UIThreadArgs args){
    ui_thread_args.start_time = args.start_time;
    ui_thread_args.end_time = args.end_time;
    ui_thread_args.simulation_time = args.simulation_time;
    ui_thread_args.ui_mutex = args.ui_mutex;
    ui_thread_args.state_ptr = args.state_ptr;
    ui_thread_args.state_mutex = args.state_mutex;
}


void UIHandler::DeclareService(std::string pipe_name){
    ui_thread_args._fd = open(pipe_name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    ui_thread_args.isService = true;
    isService = true;
}



void UIHandler::StartUIHandler(){
    ui_handler_thread = std::make_shared<std::thread>(UIThreadWork, ui_thread_args);
}


void UIHandler::StopUIHandler(){
    while(1){
        if(ui_handler_thread->joinable()){
            ui_handler_thread->join();
            break;
        }
        std::this_thread::yield();
    }
    if( isService ){
        ui_thread_args.state_mutex->lock();
        *ui_thread_args.state_ptr &= ~SIM_RUNNING;
        write(ui_thread_args._fd, ui_thread_args.state_ptr, SIGNAL_SIZE);
        close(ui_thread_args._fd);
        ui_thread_args.state_mutex->unlock();
    }
}