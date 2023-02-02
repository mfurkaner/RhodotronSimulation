#ifndef GUI_ANALYSIS_FRAME_H
#define GUI_ANALYSIS_FRAME_H


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
#include "TGComboBox.h"

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
#include "../../renderer/gui_renderer.h"
#include "../../analyzer/definitions.h"
#include "analysis_control_frames/gui_E_dist_analysis_frame.h"
#include "analysis_control_frames/gui_t_vs_E_analysis_frame.h"



namespace RhodotronSimulatorGUI::frames{

    enum AnalysisType{
        EnergyDistribution, single_e_t_vs_E
    };

    class AnalysisFrame : public TGVerticalFrame{
        TRootEmbeddedCanvas* canvas;
        const TGWindow* parent;
        Analysis::Analyzer* analyzer;

        TGComboBox* _analysisSelection;
        std::vector<AnalysisType> _analysisTypes;

        subframes::EDistributionControlFrame* _Edist_control_frame;
        subframes::TvsEControlFrame* _tvsE_control_frame;

        float _starttime = 0;
        float _endtime = 40;
        int _enum = 1;
        int _bnum = 1;

        TGFrame* active_frame;
        
        void _fillAnalysisSelection();

    public:
        AnalysisFrame(const TGWindow* p, UInt_t w, UInt_t h, Analysis::Analyzer* analyzer_);

        void SetTimeInterval(float starttime, float endtime);
        void SetEnumBnum(int _enum, int _bnum);
        void SetupAnalyzer();

        void AnalysisTypeSelected(Int_t id);

        void EnergyDistributionRequested();
        void TvsERequested();

        void SavePressed();

        void clear();


        void NavigateTo(TGFrame* child_frame);
        void OnNavigatedTo();
        void HideAll(bool hide_canvas = false);
    };

}



#endif