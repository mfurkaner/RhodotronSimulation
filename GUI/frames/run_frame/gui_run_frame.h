#ifndef GUI_RUN_FRAME_H
#define GUI_RUN_FRAME_H


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
#include "TGProgressBar.h"
#include "TGNumberEntry.h"
#include "TGSlider.h"

#include <stdio.h>
#include <string.h>
#include <vector>
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
#include <mutex>

#include "../gui_frames_common.h"
#include "../../comm/sim_server.h"
#include "../../data/gui_sim_dataprovider.h"
#include "../../renderer/gui_renderer.h"
#include "../../data/gui_data_types.h"
#include "../../msgbox/gui_msgbox_handler.h"



//namespace RhodotronSimulatorGUI::frames{

    class RunFrame : public TGVerticalFrame{
        const TGWindow* parent;
        GUISimulationHandler* sim_handler;
        DataProvider* dp;
        Renderer* renderer;


        TGButton* _runButton;
        TGButton* _stopButton;
        TGButton* _pauseButton;
        TGButton* _continueButton;

        TGHProgressBar* progressBar;
        std::shared_ptr<std::mutex> _progressBar_mutex;
        TGLabel* _status;
        std::shared_ptr<std::mutex> _status_mutex;
        TGLabel* _label;
        
        std::thread* _runCheckerThread = NULL;
        
    public:
        RunFrame(const TGWindow* p, UInt_t w, UInt_t h);

        void Setup();

        void SetSimulationHandler(GUISimulationHandler* _sim_handler){sim_handler = _sim_handler;}
        void SetRenderer(Renderer* _renderer){renderer = _renderer;}
        void SetDataProvider(DataProvider* _dp){dp = _dp;}
        
        void RunPressed();
        void StopPressed();
        void PausePressed();
        void ContinuePressed();

        void ShowRunningButtons();
        void ShowStandbyButtons();

        void RunFinished();

        static void RunCheckerThreadWork(GUISimulationHandler* _sim_handler, RunFrame* _frame);

        void OnNavigatedTo();
    };

//}



#endif