#include "sim_server.h"
#include "signal.h"
#include <sys/wait.h>
#include <sys/types.h>

#include "TSystem.h"
#include "../frames/gui_frames_common.h"


const std::string GUISimulationHandler::sim_server_pipe_name = "gui_pipe";
const std::string GUISimulationHandler::sim_stdout_redirect = "newout";
const std::string GUISimulationHandler::sim_exe = "./simrhodo.exe";
const std::vector<std::string> GUISimulationHandler::sim_args = { sim_exe, "-fd", sim_server_pipe_name};


void GUISimulationHandler::spawn_simulation(){
    if( isRunning )return;
    if (_status) _status->SetText(RhodotronSimulatorGUI::frames::Run_frame_status_title_starting.c_str());
    isRunning = true;
    extern char** environ;

    char* args[] = {(char*)sim_args[0].c_str(), (char*)sim_args[1].c_str(), (char*)sim_args[2].c_str(), nullptr};

    // file action settings
    posix_spawn_file_actions_t file_actions;
    posix_spawn_file_actions_init(&file_actions);
    posix_spawn_file_actions_addopen(&file_actions, STDOUT_FILENO, sim_stdout_redirect.c_str(),  O_RDWR|O_TRUNC|O_CREAT, 0666);
    // spawn attribute settings
    posix_spawnattr_t spawnattr;
    posix_spawnattr_init(&spawnattr);
    // spawn the simulation
    int res = posix_spawn(&_sim_pid, sim_exe.c_str(), &file_actions, nullptr, args, environ);   

    if (res != 0){
        perror("posix_spawn error");
    }
    posix_spawn_file_actions_destroy(&file_actions);
}

void GUISimulationHandler::kill_simulation(){
    int status;
    int cpid = waitpid(_sim_pid, &status, WNOHANG);
    if (cpid < 0) return;
    if (WIFEXITED(status) || WIFSTOPPED(status)) {
        return;
    }
    std::cout << getpid() << " "<<_sim_pid << " " << WIFSIGNALED(status) << std::endl; 
    kill(_sim_pid, SIGTERM);
    cpid = waitpid(_sim_pid, &status, 0);
    std::cout << getpid() << " "<<_sim_pid << " " << WIFSIGNALED(status) << std::endl; 
}

void GUISimulationHandler::sim_server_work(SimulationServerWorkerArgs worker_args){

    bool reportBar = worker_args.reportProgressBar;
    bool reportStatus = worker_args.reportStatus;

    int _fd = open_pipe(worker_args.pipe_name.c_str());
    if (_fd < 0){
        perror("open_pipe failed in sim_server_work");
        return;
    }

    uint8_t recvd_signal;

    ssize_t n ;
    bool isRunning = true;
    while ( isRunning ){
        n = read(_fd, &recvd_signal, SIGNAL_SIZE);
        if ( n <= 0){
            continue;
        }
        
        std::bitset<8> a(recvd_signal /* & SIM_WORK_MASK */);
        if (reportBar){
            worker_args.progressbar->SetPosition(recvd_signal & SIM_WORK_MASK);
        }
        if (reportStatus){
            worker_args.status->SetText(RhodotronSimulatorGUI::frames::Run_frame_status_title_running.c_str());
        }
        if ( (recvd_signal & SIM_RUNNING) == 0){
            isRunning = false;
            worker_args.owner->isRunning = false;
            break;
        }
    }
    if (reportStatus){
        worker_args.status->SetText(RhodotronSimulatorGUI::frames::Run_frame_status_title_finished.c_str());
    }

    close_pipe(_fd, worker_args.pipe_name.c_str());
//    delete worker_args;
    return;
}

void GUISimulationHandler::spawn_server(bool reportProgressBar, bool reportStatus){
    SimulationServerWorkerArgs server_args;
    server_args.pipe_name = sim_server_pipe_name;
    server_args.progressbar = _progressbar;
    server_args.status = _status;
    server_args.owner = this;
    server_args.reportProgressBar = reportProgressBar;
    server_args.reportStatus = reportStatus;

    if ( worker != NULL ){
        if(worker->joinable()){
            worker->join();
        }
        delete worker;
    }
    
    worker = new std::thread(GUISimulationHandler::sim_server_work, server_args);
}

void GUISimulationHandler::join_server(){
    while(!worker->joinable());
    worker->join();
}

bool GUISimulationHandler::server_joinable(){
    return worker->joinable();
}

void GUISimulationHandler::kill_server(){
    //pthread_kill(worker, SIGKILL);
    worker->detach();
}

void GUISimulationHandler::set_progress_bar(TGProgressBar* progressbar){
    _progressbar = progressbar;
}

void GUISimulationHandler::set_status_label(TGLabel* status){
    _status = status;
}

bool GUISimulationHandler::IsRunning() const { return isRunning; }

int GUISimulationHandler::open_pipe(const char* pipe_name){
    return open(pipe_name, O_RDONLY | O_CREAT | O_TRUNC, 0666);
}


void GUISimulationHandler::close_pipe(const int _fd, const char* pipe_name){
    if (close(_fd) != 0) {
        std::string err = "close pipe failed in close_pipe ";
        err += pipe_name;
        perror(err.c_str());
    }
    if ( unlink(pipe_name) != 0){
        std::string err = "unlink pipe failed in close_pipe ";
        err += pipe_name;
        perror(err.c_str());
    }
}