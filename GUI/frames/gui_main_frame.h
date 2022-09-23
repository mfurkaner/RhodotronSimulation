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
#include "gui_main_buttons.h"
#include "gui_config_frame.h"


namespace RhodotronSimulatorGUI::frames{


    #define MAIN_FRAME_W 500
    #define MAIN_FRAME_H 500

    #define MAIN_BUTTON_FRAME_W 500
    #define MAIN_BUTTON_FRAME_H 500

    #define CONFIG_FRAME_W 500
    #define CONFIG_FRAME_H 500
    #define CONFIG_FRAME_LINE_W 150
    #define CONFIG_FRAME_LINE_H 50


    class MainFrame : public TGMainFrame {

        std::unique_ptr<MainButtonsFrame> main_buttons_frame;
        std::unique_ptr<ConfigurationFrame> config_frame;



        std::string _config_file_path = "config.ini";

    public:
        MainFrame(const TGWindow*p, UInt_t w, UInt_t h);
        ~MainFrame();

        void SaveConfigPressed();
        void LoadConfigPressed();
        void SaveGifPressed();
        void RunPressed();
        void StopPressed();
        void QuitPressed();

    };

}

#endif