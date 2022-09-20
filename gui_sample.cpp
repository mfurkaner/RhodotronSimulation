#include "TGFrame.h"
#include "TGButton.h"
#include "TGText.h"
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

#include "inc/gui/signal.h"


class GUI : public TGMainFrame {

    public:

    GUI(const TGWindow *p, UInt_t w, UInt_t h);

    void CloseWindow();

    void StartSimulation();


    virtual ~GUI();

};


GUI::GUI(const TGWindow *p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h) {
    SetCleanup(kDeepCleanup);
    
    // Setup the main frame
    TGHorizontalFrame* frame = new TGHorizontalFrame( this, 200, 100);

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
    frame->AddFrame( simulate_button, simulate_layout);

    this->AddFrame( frame, frame_layout );

    SetName("RhodoSim_GUI");
    SetWindowName("RhodoSim GUI");

    MapSubwindows();

    Resize( GetDefaultSize());
    MapWindow();
}

void GUI::StartSimulation(){
    extern char** environ;
    std::cout << "Simulation started" << std::endl;
    
    pid_t pid;
    int res;

    char* args[] = {"./simulation", "-fd", "gui_pipe", nullptr};

    posix_spawn_file_actions_t file_actions;
    posix_spawn_file_actions_init(&file_actions);
    posix_spawn_file_actions_addopen(&file_actions, STDOUT_FILENO, "newout",  O_WRONLY|O_APPEND, 0);
    
    posix_spawnattr_t spawnattr;

    posix_spawnattr_init(&spawnattr);

    res = posix_spawn(&pid, "./simulation", &file_actions, nullptr, args, environ);   
    if (res != 0){
        perror("posix_spawn");
    }
    posix_spawn_file_actions_destroy(&file_actions);
    std::cout <<"Spawn res :" << res << std::endl;

    int _fd = open("gui_pipe", O_RDONLY);

    if (_fd < 0){
        std::cout << "Failed to open: " << _fd      << std::endl;
        perror("open gui_pipe failed.");
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

}


void GUI::CloseWindow(){
    DeleteWindow();
}

GUI::~GUI(){
    DeleteWindow();
}

void gui_sample(){
    new GUI( gClient->GetRoot(), 200, 200);
}