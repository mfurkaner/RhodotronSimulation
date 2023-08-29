#ifndef SIM_SERVER_H
#define SIM_SERVER_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <spawn.h>
#include <sched.h>
#include <signal.h>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <vector>
#include <thread>

#include "TGProgressBar.h"
#include "TGLabel.h"

class GUISimulationHandler;

struct SimulationServerWorkerArgs{
    std::shared_ptr<bool> terminate;
    std::string pipe_name;
    TGProgressBar* progressbar;
    TGLabel* status;
    GUISimulationHandler* owner;
    bool reportProgressBar = true;
    bool reportStatus = true;
};
 
class GUISimulationHandler {
    //pthread_t worker;
    std::thread* worker = NULL;
    std::shared_ptr<bool> worker_terminate = std::make_shared<bool>(false);
    pid_t _sim_pid;
    TGProgressBar* _progressbar;
    TGLabel* _status;

    bool isRunning = false;

public:
    const static std::string sim_server_pipe_name;
    const static std::string sim_stdout_redirect;
    const static std::string sim_exe;
    const static std::vector<std::string> sim_args;

    void spawn_simulation();
    void kill_simulation();
    static void sim_server_work(SimulationServerWorkerArgs worker_args);

    void spawn_server(bool reportProgressBar = true, bool reportStatus = true);
    void join_server();
    bool server_joinable();
    void kill_server();

    void set_progress_bar(TGProgressBar* progressbar);
    void set_status_label(TGLabel* status);

    bool IsRunning() const ;

    static int open_pipe(const char* path);
    static void close_pipe(const int _fd, const char* pipe_name);
};

#endif