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


#include "comm/signal.h"
#include "frames/gui_main_frame.h"
#include "frames/gui_main_frame.cpp"
#include "frames/ribbon_frame/gui_main_buttons.cpp"
#include "frames/config_frame/gui_config_frame.cpp"
#include "frames/config_frame/B_config_frame/gui_B_config_frame.cpp"
#include "frames/config_frame/E_config_frame/gui_E_config_frame.cpp"
#include "frames/config_frame/gun_config_frame/gui_gun_config_frame.cpp"
#include "frames/config_frame/sim_config_frame/gui_sim_config_frame.cpp"
#include "frames/render_frame/gui_render_frame.cpp"
#include "frames/analysis_frame/gui_analysis_frame.cpp"
#include "frames/run_frame/gui_run_frame.cpp"
#include "comm/sim_server.cpp"
#include "renderer/gui_renderer.cpp"
#include "data/gui_sim_dataprovider.cpp"





void gui_refactored(){
    gSystem->cd("..");
    //gSystem->DispatchOneEvent();
    gSystem->ProcessEvents();
    new RhodotronSimulatorGUI::frames::MainFrame( gClient->GetRoot());
}

int main(){
    gSystem->cd("..");
    //gSystem->DispatchOneEvent();
    gSystem->ProcessEvents();
    new RhodotronSimulatorGUI::frames::MainFrame( gClient->GetRoot());
}