#ifndef GUI_MAIN_FRAME_H
#define GUI_MAIN_FRAME_H


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

#include "TSystem.h"
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


#include "../comm/signal.h"
#include "../comm/sim_server.h"
#include "gui_frames_common.h"
#include "ribbon_frame/gui_main_buttons.h"
#include "config_frame/gui_config_frame.h"
#include "render_frame/gui_render_frame.h"
#include "../renderer/gui_renderer.cpp"


namespace RhodotronSimulatorGUI::frames{

    class MainFrame : public TGMainFrame {
        GUISimulationHandler sim_handler;
        MainButtonsFrame* main_buttons_frame;
        ConfigurationFrame* config_frame;
        RenderFrame* render_frame;

        TGFrame* active_frame;



        std::string _config_file_path = "config.ini";
        std::string _old_config_file_path = "gui_config_backup.ini";


    public:
        MainFrame(const TGWindow*p, UInt_t w, UInt_t h);
        ~MainFrame();

        void SaveConfigPressed();
        void LoadConfigPressed();
        void ConfigurationPressed();
        void RenderPressed();
        void RunPressed();
        void StopPressed();
        void QuitPressed();


        void NavigateTo(TGFrame* childFrame, std::string hideButton = "");

    };

}

#endif