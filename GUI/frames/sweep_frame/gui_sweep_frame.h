#ifndef GUI_SWEEP_FRAME_H
#define GUI_SWEEP_FRAME_H


#include "TSystem.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGTextEntry.h"
#include "TGLViewer.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TGProgressBar.h"
#include "TGNumberEntry.h"
#include "TGComboBox.h"
#include "TMultiGraph.h"

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
#include "../config_frame/gui_config_frame.h"
#include "../../renderer/gui_renderer.h"
#include "../../analyzer/definitions.h"
#include "../../comm/sim_server.h"
#include "../../data/gui_sim_dataprovider.h"
#include "sweep_control_frames/gui_sweep_phaselag_frame.h"



namespace RhodotronSimulatorGUI::frames{

    enum SweepType{
        PhaseLagSweep
    };


    class SweepFrame : public TGVerticalFrame{
        const TGWindow* parent;
        Analysis::Analyzer* analyzer;
        GUISimulationHandler* sim_handler;
        data::DataProvider* dataProvider;

        ConfigurationFrame* config_frame;

        TGComboBox* _sweepSelection;
        std::vector<SweepType> _sweepTypes;

        subframes::PhaseLagSweepControlFrame* _phaselag_control_frame;

        float _phaselag_start;
        float _phaselag_end;

        TGFrame* active_frame;
        
        void _fillSweepSelection();

    public:
        SweepFrame(const TGWindow* p, UInt_t w, UInt_t h, Analysis::Analyzer* analyzer_, GUISimulationHandler* sim_handler_, data::DataProvider* dataProvider_, ConfigurationFrame* config_frame_);

        void SweepTypeSelected(Int_t id);

        void NavigateTo(TGFrame* child_frame);
        void OnNavigatedTo();
        void HideAll();
    };
}



#endif