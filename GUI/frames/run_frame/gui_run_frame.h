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

#include "../gui_frames_common.h"
#include "../../comm/sim_server.h"



namespace RhodotronSimulatorGUI::frames{

    class RunFrame : public TGVerticalFrame{
        TGHProgressBar* progressBar;
        const TGWindow* parent;
        GUISimulationHandler* sim_handler;


        TGButton* _runButton;
        TGButton* _stopButton;
        TGLabel* _status;
        TGLabel* _label;
        
    public:
        RunFrame(const TGWindow* p, UInt_t w, UInt_t h);

        void SetSimulationHandler(GUISimulationHandler* _sim_handler){sim_handler = _sim_handler;}
        
        void RunPressed();
        void StopPressed();

    };

}



#endif