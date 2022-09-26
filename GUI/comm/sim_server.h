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


class GUISimulationHandler {
    pthread_t worker;
    pid_t _sim_pid;

public:
    const static std::string sim_server_pipe_name;
    const static std::string sim_stdout_redirect;
    const static std::string sim_exe;
    const static std::vector<std::string> sim_args;

    void spawn_simulation();
    void kill_simulation();
    static void* sim_server_work(void* worker_args);

    void spawn_server();
    void join_server();
    void kill_server();

    static int open_pipe(const char* path);
    static void close_pipe(const int _fd, const char* pipe_name);
};

struct SimulationServerWorkerArgs{
    std::string pipe_name;
};

#endif