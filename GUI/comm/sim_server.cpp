#include "sim_server.h"
#include "signal.h"
#include <sys/wait.h>
#include <sys/types.h>

#include "TSystem.h"


const std::string GUISimulationHandler::sim_server_pipe_name = "gui_pipe";
const std::string GUISimulationHandler::sim_stdout_redirect = "newout";
const std::string GUISimulationHandler::sim_exe = "./simrhodo.exe";
const std::vector<std::string> GUISimulationHandler::sim_args = { sim_exe, "-fd", sim_server_pipe_name};


void GUISimulationHandler::spawn_simulation(){
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


void* GUISimulationHandler::sim_server_work(void* worker_args){

    SimulationServerWorkerArgs *args = (SimulationServerWorkerArgs*)worker_args;

    int _fd = open_pipe(args->pipe_name.c_str());
    if (_fd < 0){
        perror("open_pipe failed in sim_server_work");
        return nullptr;
    }

    uint8_t recvd_signal;

    ssize_t n ;
    bool isRunning = true;
    while ( isRunning ){
        n = read(_fd, &recvd_signal, SIGNAL_SIZE);
        if ( n <= 0){
            continue;
        }
        
        std::bitset<8> a(recvd_signal & SIM_WORK_MASK);
        std::cout << a << std::endl;
        args->progressbar->SetPosition(recvd_signal & SIM_WORK_MASK);
        if ( (recvd_signal & SIM_RUNNING) == 0){
            isRunning = false;
            break;
        }
    }
    std::cout << "Simulation is finished" << std::endl;
    isRunning = false;

    close_pipe(_fd, args->pipe_name.c_str());
    delete args;

    return nullptr;
}

void GUISimulationHandler::spawn_server(){
    SimulationServerWorkerArgs* server_args = new SimulationServerWorkerArgs();
    server_args->pipe_name = sim_server_pipe_name;
    server_args->progressbar = _progressbar;
    pthread_create(&worker, NULL, GUISimulationHandler::sim_server_work, (void*)server_args);
}

void GUISimulationHandler::join_server(){
    void* a;
    pthread_join(worker, &a);
}

void GUISimulationHandler::kill_server(){
    pthread_kill(worker, SIGKILL);
}

void GUISimulationHandler::set_progress_bar(TGProgressBar* progressbar){
    _progressbar = progressbar;
}


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