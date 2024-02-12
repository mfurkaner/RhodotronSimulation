#include <iostream>
#include "TSystem.h"
#include "frames/MainFrame.h"

#define INTERPRETED 0

#if (INTERPRETED == 0)
    #include "comm/signal.h"
    #include "TApplication.h"
#endif

#if (INTERPRETED == 1)
    #include "comm/signal.h"
    #include "frames/ribbon_frame/gui_main_buttons.cpp"
    #include "frames/config_frame/gui_config_frame.cpp"
    #include "frames/config_frame/B_config_frame/gui_B_config_frame.cpp"
    #include "frames/config_frame/E_config_frame/gui_E_config_frame.cpp"
    #include "frames/config_frame/gun_config_frame/gui_gun_config_frame.cpp"
    #include "frames/config_frame/sim_config_frame/gui_sim_config_frame.cpp"
    #include "frames/render_frame/gui_render_frame.cpp"
    #include "frames/analysis_frame/gui_analysis_frame.cpp"
    #include "frames/analysis_frame/analysis_control_frames/gui_E_dist_analysis_frame.cpp"
    #include "frames/analysis_frame/analysis_control_frames/gui_t_vs_E_analysis_frame.cpp"
    #include "frames/run_frame/gui_run_frame.cpp"
    #include "frames/sweep_frame/gui_sweep_frame.cpp"
    #include "frames/sweep_frame/sweep_control_frames/gui_sweep_base_subframe.cpp"
    #include "frames/sweep_frame/sweep_control_frames/gui_sweep_phaselag_frame.cpp"
    #include "comm/sim_server.cpp"
    #include "renderer/gui_renderer.cpp"
    #include "analyzer/gui_analyzer.cpp"
    #include "data/gui_sim_dataprovider.cpp"
    #include "msgbox/gui_msgbox_handler.cpp"
    #include "../SIM/basic/vector.cpp"
#endif

#if(INTERPRETED == 0)

int main(int argc, char **argv){
    TApplication theApp("App", &argc, argv);
    //gSystem->cd("..");
    gSystem->ProcessEvents();
    //gSystem->DispatchOneEvent();
    auto main_frame = new MainFrame( gClient->GetRoot());
    main_frame->SetupFrame();
    theApp.Run();
    return 0;

}
#else
void gui_refactored(){
    gSystem->cd("..");
    //gSystem->DispatchOneEvent();
    gSystem->ProcessEvents();
    auto frame = new MainFrame( gClient->GetRoot());
    frame->SetupFrame();
}
#endif

