#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <spawn.h>
#include <sched.h>
#include <iostream>
#include <iomanip>
#include <bitset>

#include "TGFrame.h"
#include "TGButton.h"
#include "TGText.h"
#include "TGTextEntry.h"
#include "TGLViewer.h"
#include "TGPicture.h"

#include "inc/gui/signal.h"


class GUI : public TGMainFrame {
    pthread_t sim_server;


    TGPicture* gif_output;
    public:

    GUI(const TGWindow *p, UInt_t w, UInt_t h);

    void CloseWindow();

    void StartSimulation();
    static void* SIM_SERVER_THREAD_WORK(void* sim_server_args);


    virtual ~GUI();

};


GUI::GUI(const TGWindow *p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h) {
    SetCleanup(kDeepCleanup);
    
    // Setup the main frame
    TGHorizontalFrame* frame = new TGHorizontalFrame( this, 500, 500);

    TGLayoutHints* frame_layout = new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2);

    // Setup the exit button
    TGTextButton* exit_button = new TGTextButton(frame, "Exit");

    exit_button->Connect( "Clicked()", "GUI", this, "CloseWindow()");
    TGLayoutHints* exit_layout = new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 55);

    // Setup the simulate button
    TGTextButton* simulate_button = new TGTextButton(frame, "Simulate");

    simulate_button->Connect("Clicked()", "GUI", this, "StartSimulation()");
    TGLayoutHints* simulate_layout = new TGLayoutHints(kLHintsCenterX, 5,5, 55, 5);

    frame->AddFrame( exit_button, exit_layout);
    frame->AddFrame( simulate_button, exit_layout);

    this->AddFrame( frame, frame_layout );

    


    SetName("RhodoSim_GUI");
    SetWindowName("RhodoSim GUI");

    MapSubwindows();

    Resize( GetDefaultSize() );
    MapWindow();
}

void GUI::StartSimulation(){
    extern char** environ;
    std::cout << "Simulation started" << std::endl;
    
    pid_t pid;
    int res;

    char* args[] = {(char*)"./simulation", (char*)"-fd", (char*)"gui_pipe", nullptr};

    posix_spawn_file_actions_t file_actions;
    posix_spawn_file_actions_init(&file_actions);
    posix_spawn_file_actions_addopen(&file_actions, STDOUT_FILENO, "newout",  O_RDWR|O_TRUNC|O_CREAT, 0666);
    
    posix_spawnattr_t spawnattr;

    posix_spawnattr_init(&spawnattr);

    res = posix_spawn(&pid, "./simulation", &file_actions, nullptr, args, environ);   

    if (res != 0){
        perror("posix_spawn error");
        std::cout <<"Spawn res : " << res << std::endl;
        return;
    
    }
    posix_spawn_file_actions_destroy(&file_actions);

    void* pipe_name = malloc(strlen("gui_pipe"));
    strcpy((char*)pipe_name, "gui_pipe");

    pthread_create(&sim_server, NULL, GUI::SIM_SERVER_THREAD_WORK, (void*) "gui_pipe");
    
}

void* GUI::SIM_SERVER_THREAD_WORK(void* sim_server_args){   

    int _fd = open((char*)sim_server_args, O_RDONLY | O_CREAT );

    if (_fd < 0){
        std::cout << "Failed to open : " << _fd      << std::endl;
        perror("open gui_pipe failed.");
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
        std::bitset<8> a(recvd_signal );
        std::cout << a << " : " << ( (recvd_signal & SIM_WORK_MASK) != SIM_WORK_MASK ? "Calculating" : "Not calculating" ) << std::endl;
        if ( (recvd_signal & SIM_RUNNING) == 0){
            isRunning = false;
            break;
        }
        else {
            std::cout << 100*((float)(recvd_signal & SIM_WORK_MASK))/SIM_WORK_MASK << "% done." << std::endl;
        }
        recvd_signal = 0;
    }
    std::cout << "Simulation is finished" << std::endl;
    isRunning = false;


    close(_fd);
    // close frees sim_server_args for us
    //free(sim_server_args);
    return nullptr;
}

void GUI::CloseWindow(){
    void* a;
    pthread_join(sim_server, &a);
    DeleteWindow();
}

GUI::~GUI(){
    void* a;
    pthread_kill(sim_server,9);
    DeleteWindow();
}

void gui_sample(){
    new GUI( gClient->GetRoot(), 500, 500);
}