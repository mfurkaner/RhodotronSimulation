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
#include <fstream>

#include "TGFrame.h"
#include "TGButton.h"
#include "TGTextView.h"
#include "TGLabel.h"
#include "TGTextEntry.h"
#include "TGLViewer.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TImage.h"
#include "TASImage.h"


#include "inc/gui/signal.h"
#include "inc/gui/sim_server.h"
#include "inc/gui/sim_server.cpp"


class GUI : public TGMainFrame {

    GUISimulationHandler sim_handler;
    TRootEmbeddedCanvas* gif_output;

    std::vector<std::string> configs = {
        "emax", "ein", "targeten", "freq", "phaselag", "starttime",
        "endtime", "dt", "guntime", "gunperiod", "enum", "bunchnum",
        "r1", "r2", "epath", "bpath", "cfield", "ppath", "starttime",
        "multh", "thcount", "magrotation", "addmagnet", "output"
    };
    std::vector<TGTextEntry*> config_entries;

    TGVerticalFrame* SetupConfigurationInputs();
    TRootEmbeddedCanvas* SetupGifCanvas();

    public:

    GUI(const TGWindow *p, UInt_t w, UInt_t h);

    void CloseWindow();

    void StartSimulation();
    void PushConfigurationsToFile();
    virtual ~GUI();

};


TGVerticalFrame* GUI::SetupConfigurationInputs(){
    TGVerticalFrame *verticalFrame = new TGVerticalFrame( this, 500, 500);

    TGLayoutHints* right_layout = new TGLayoutHints(kLHintsRight, 5, 5, 5, 5);
    TGLayoutHints* left_layout = new TGLayoutHints(kLHintsLeft, 5, 5, 5, 5);
    TGLayoutHints* bottom_layout = new TGLayoutHints(kLHintsBottom, 5, 5, 5, 5);

    for( int i = 0; i < configs.size(); i++ ){
        TGHorizontalFrame* config_line_frame = new TGHorizontalFrame( verticalFrame, 150, 50);
        TGTextEntry* entry = new TGTextEntry( "" , config_line_frame, 9);

        config_entries.push_back(entry);

        //entry->Connect("ReturnPressed()", "GUI", this, "asd");
        entry->Resize(100, 25);
        TGLabel* tag = new TGLabel(config_line_frame, configs[i].c_str() );

        config_line_frame->AddFrame(tag, bottom_layout);
        config_line_frame->AddFrame(entry, right_layout);

        verticalFrame->AddFrame(config_line_frame, right_layout);
    }
    
    return verticalFrame;
}

TRootEmbeddedCanvas* GUI::SetupGifCanvas(){
        /*
    gif_output = new TRootEmbeddedCanvas("output gif", frame, 200, 200);

    gif_output->GetCanvas()->cd(1);
    //TImage *image[100];
    TASImage *image = new TASImage("1.gif", TImage::kAnimGif);
    
    for (int i = 0; i < 100; i++){
        image[i] = TImage::Open("1.gif", TImage::kAnimGif).;
    }
    for (int i = 0; i < 100; i++){
        image->Draw();
        gif_output->GetCanvas()->cd(1)->Update();
    }
    
    image->Draw();
    gif_output->GetCanvas()->cd(1)->Update();
    */
    return new TRootEmbeddedCanvas();
}


void GUI::PushConfigurationsToFile() {
    std::ofstream file;
    file.open("config.ini", std::ios::out);

    file <<     "#                  RhodoSim Configuration File\n"
                "# ================================================================\n"
                "#    M.Furkan Er                                     22/09/2022   \n"
                "# ================================================================\n#\n"
                "# emax = Maximum electric field strength (MV/m)\n"
                "# ein = Energy of electrons coming out of the gun (MeV)\n" 
                "# targeten = Max energy on the output gif (MeV)\n"
                "# freq = Frequency of the RF field (MHz)\n"
                "# phaselag = phase lag of the first electrons (degree)\n"
                "# starttime = ns to start the simulation (ns)\n"
                "# endtime = ns to run the simulation (ns)\n"
                "# dt = time interval to do the calculations (ns)\n"
                "# guntime = how long a gun pulse is (ns)\n"
                "# gunperiod = time between two gun pulses (ns)\n"
                "# enum = number of electrons to simulate in a bunch\n"
                "# bunchnum = number of times the gun fires\n"
                "# r1 = radius of the inner cylinder (m)\n"
                "# r2 = radius of the outer cylinder (m)\n"
                "# epath = path to store the electric field data\n"
                "# bpath = path to store the magnetic field data\n"
                "# cfield = path to store the settings\n"
                "# ppath = path to store electron data\n"
                "# starttime = time to start firing the gun (ns)\n"
                "# multh = enable or disable multitheading\n"
                "# thcount = set the maximum thread to be used\n"
                "# magrotation = degrees of rotation to enter each magnet \n"
                "# addmagnet = takes 3 input. (B , R, < Radial distance of center >)\n"
                "# output = output file name \n" << std::endl;
    
    for ( int i = 0; i < configs.size(); i++ ) {
        file << configs.at(i) << " = " << config_entries.at(i)->GetText() << (i + 1 < configs.size() ? "\n" : "");
    }
    file.close();
}


GUI::GUI(const TGWindow *p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h) {
    SetCleanup(kDeepCleanup);
    
    // Setup the button frame
    TGHorizontalFrame* button_frame = new TGHorizontalFrame( this, 500, 500);
    TGLayoutHints* button_frame_layout = new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2);

    // Setup the input frame
    TGVerticalFrame* input_frame = SetupConfigurationInputs();

    TGLayoutHints* input_frame_layout = new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2); 

    //SetupGifCanvas()


    // Setup the exit button
    TGTextButton* exit_button = new TGTextButton(button_frame, "Exit");
    TGLayoutHints* exit_layout = new TGLayoutHints(kLHintsCenterX, 5, 5, 5, 5);
    exit_button->Connect( "Clicked()", "GUI", this, "CloseWindow()");

    // Setup the save button
    TGTextButton* save_button = new TGTextButton(button_frame, "Save");
    save_button->Connect( "Clicked()", "GUI", this, "PushConfigurationsToFile()");

    // Setup the simulate button
    TGTextButton* simulate_button = new TGTextButton(button_frame, "Simulate");
    simulate_button->Connect("Clicked()", "GUI", this, "StartSimulation()");

    button_frame->AddFrame( exit_button, exit_layout);
    button_frame->AddFrame( save_button, exit_layout);
    button_frame->AddFrame( simulate_button, exit_layout);
    //frame->AddFrame( gif_output, simulate_layout);

    this->AddFrame( button_frame, button_frame_layout );
    this->AddFrame( input_frame, input_frame_layout );


    SetName("RhodoSim_GUI");
    SetWindowName("RhodoSim GUI");

    MapSubwindows();

    Resize( GetDefaultSize() );
    MapWindow();
}

void GUI::StartSimulation(){

    std::cout << "Simulation started" << std::endl;

    PushConfigurationsToFile();

    sim_handler.spawn_server();
    sim_handler.spawn_simulation();

}

void GUI::CloseWindow(){
    void* a;
    sim_handler.join_server();
    DeleteWindow();
}

GUI::~GUI(){
    void* a;
    sim_handler.kill_server();
    DeleteWindow();
}

void gui_sample(){
    new GUI( gClient->GetRoot(), 500, 500);
}