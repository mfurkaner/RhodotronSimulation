#ifndef GUI_GUN_CONFIG_FRAME_H
#define GUI_GUN_CONFIG_FRAME_H



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

    class GunConfigurationFrame : public TGVerticalFrame{
        const TGWindow* parent;

        TGLabel* _frame_title;

        TGNumberEntry* _Ein_entry;
        TGNumberEntry* _targetE_entry;
        TGNumberEntry* _T_entry;
        TGNumberEntry* _tg_entry;
        TGNumberEntry* _bnum_entry;
        TGNumberEntry* _enum_entry;

        TGLabel* _Ein_entry_label;
        TGLabel* _targetE_entry_label;
        TGLabel* _T_entry_label;
        TGLabel* _tg_entry_label;
        TGLabel* _bnum_entry_label;
        TGLabel* _enum_entry_label;

        TGIcon* _gun_description_canvas;

        TGHorizontalFrame* _init_Ein_icon_frame();
        TGHorizontalFrame* _init_gun_times_frame();
        TGHorizontalFrame* _init_particle_samples_frame();
    public:

        GunConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h);

        std::string ProduceGunConfiguration();
        void SetGunConfiguration(std::string config);

        const int GetEnum(){ return (_enum_entry ? (int)_enum_entry->GetNumber() : 0); }
        const int GetBnum(){ return (_bnum_entry ? (int)_bnum_entry->GetNumber() : 0); }
        const double GetTargetEn(){ return (_targetE_entry ? _targetE_entry->GetNumber() : 0); }
    };
}


#endif