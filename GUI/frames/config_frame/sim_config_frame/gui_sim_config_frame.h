#ifndef GUI_SIM_CONFIG_FRAME_H
#define GUI_SIM_CONFIG_FRAME_H



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

namespace RhodotronSimulatorGUI::frames::subframes{

    class SimConfigurationFrame : public TGVerticalFrame{

        const TGWindow* parent;

        TGLabel* _frame_title;

        TGNumberEntry* _t0_entry;
        TGNumberEntry* _tf_entry;
        TGNumberEntry* _dt_entry;
        TGNumberEntry* _thnum_entry;

        TGCheckButton* _multh_enable_button;

        TGTextEntry* _epath_entry;
        TGTextEntry* _bpath_entry;
        TGTextEntry* _ppath_entry;
        TGTextEntry* _cpath_entry;

        TGLabel* _t0_entry_label;
        TGLabel* _tf_entry_label;
        TGLabel* _dt_entry_label;
        TGLabel* _thnum_entry_label;
        TGLabel* _epath_entry_label;
        TGLabel* _bpath_entry_label;
        TGLabel* _ppath_entry_label;
        TGLabel* _cpath_entry_label;

        TGHorizontalFrame* _init_time_entry_frame();
        TGVerticalFrame* _init_mt_control_frame();
        TGHorizontalFrame* _init_log_path_entry_frame();

    public:

        SimConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h);

        std::string ProduceSimConfiguration();
        void SetSimConfiguration(std::string config);

        const float GetStartTime(){ return (_t0_entry ? _t0_entry->GetNumber() : 0);}
        const float GetEndTime(){ return (_tf_entry ? _tf_entry->GetNumber() : 0);}
        const std::string GetEPath(){ return (_epath_entry ? _epath_entry->GetText(): "xy/rf.txt");}
        const std::string GetBPath(){ return (_bpath_entry ? _bpath_entry->GetText(): "xy/magnet.txt");}
        const std::string GetPPath(){ return (_ppath_entry ? _ppath_entry->GetText(): "xy/paths/");}
        const std::string GetCPath(){ return (_cpath_entry ? _cpath_entry->GetText(): "xy/settings.txt");}
    };
}

#endif