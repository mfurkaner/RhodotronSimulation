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
#include "run_frame/gui_run_frame.h"
#include "analysis_frame/gui_analysis_frame.h"
#include "sweep_frame/gui_sweep_frame.h"
#include "../data/gui_sim_dataprovider.h"
#include "../analyzer/gui_analyzer.h"
#include "../msgbox/gui_msgbox_handler.h"


//namespace RhodotronSimulatorGUI::frames{

    class MainFrame : public TGMainFrame {
        GUISimulationHandler sim_handler;
        Renderer renderer;
        DataProvider dataProvider;
        Analyzer analyzer;

        MainButtonsFrame* main_buttons_frame;

        ConfigurationFrame* config_frame;
        RunFrame* run_frame;
        RenderFrame* render_frame;
        AnalysisFrame* analysis_frame;
        SweepFrame* sweep_frame;

        TGFrame* active_frame;


        std::string _config_file_path = "config.ini";
        std::string _completed_config_file_path = "settings.txt";
        std::string _old_config_file_path = "gui_config_backup.ini";

        UInt_t w = MAIN_FRAME_W;
        UInt_t h = MAIN_FRAME_H;

    public:
        MainFrame(const TGWindow*p);
        ~MainFrame();

        void SetupFrame();
        
        void ClearData();
        void FillData();
        
        void ConfigurationPressed();
        void RenderPressed();
        void AnalyzePressed();
        void SweepPressed();
        void SimulatePressed();
        void QuitPressed();

        void NavigateTo(TGFrame* childFrame);
        void NavigateToConfigFrame();
        void NavigateToRenderFrame();
        void NavigateToAnalysisFrame();
        void NavigateToSweepFrame();
        void NavigateToSimalateFrame();

    private:
      TGTextButton        *abutton[2];

ClassDef (MainFrame,0)
    };


//}

#endif
