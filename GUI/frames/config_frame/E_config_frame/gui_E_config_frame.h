#ifndef GUI_E_CONFIG_FRAME_H
#define GUI_E_CONFIG_FRAME_H



#include "TSystem.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGTextView.h"
#include "TGLabel.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"
#include "TGListBox.h"
#include "TGLViewer.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TImage.h"
#include "TASImage.h"
#include "TGIcon.h"

#include <stdio.h>
#include <string.h>
#include <vector>
#include <tuple>
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

#include "../../gui_frames_common.h"

//namespace RhodotronSimulatorGUI::frames::subframes{

    class EConfigurationFrame : public TGVerticalFrame{
        const TGWindow* parent;

        TGLabel* _frame_title;
        TGLabel* _cavity_desc_title;
        TGLabel* _rf_desc_title;

        TGNumberEntry* _r1_entry;
        TGNumberEntry* _r2_entry;
        TGNumberEntry* _Emax_entry;
        TGNumberEntry* _freq_entry;
        TGNumberEntry* _phase_lag_entry;

        TGLabel* _r1_entry_label;
        TGLabel* _r2_entry_label;
        TGLabel* _Emax_entry_label;
        TGLabel* _freq_entry_label;
        TGLabel* _phase_lag_entry_label;

        TGIcon* _cavity_description_canvas;
        TGIcon* _rf_description_canvas;

        TGHorizontalFrame* _init_cavity_desc_frame();
        TGHorizontalFrame* _init_rf_desc_frame();

    public:

        EConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h);

        std::string ProduceEConfiguration();
        void SetEConfiguration(std::string config);
        void SetPhaseLag(double phase);

        float GetR1(){ return (_r1_entry ? _r1_entry->GetNumber() : 0);}
        float GetR2(){ return (_r2_entry ? _r2_entry->GetNumber() : 0);}
    };


//}



#endif