#ifndef GUI_B_CONFIG_FRAME_H
#define GUI_B_CONFIG_FRAME_H



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

    struct MagnetConfigurationDetails{
        float B;
        float R;
        float r;

        MagnetConfigurationDetails(){}
        MagnetConfigurationDetails(std::string config);

        std::string GetDescription() const { 
            char temp[100];
            snprintf(temp, 100, "B=%.5f(T)   R=%.3f(m)   r=%.3f(m)", B, R, r);
            return std::string(temp); 
        };

        std::string GetConfiguration() const {
            return std::to_string(B) + " , " + std::to_string(R) + " , <" + std::to_string(r) + ">";
        }
    };

    class BConfigurationFrame : public TGVerticalFrame{
        const TGWindow* parent;

        TGLabel* _frame_title;

        std::vector<MagnetConfigurationDetails> _magnets;
        TGListBox* _magnets_listbox;

        TGTextButton* _add_magnet_button;
        TGTextButton* _save_magnet_button;
        TGTextButton* _delete_magnet_button;

        TGNumberEntry* _B_entry;
        TGNumberEntry* _R_entry;
        TGNumberEntry* _r_entry;
        TGNumberEntry* _mag_rotation_entry;
        
        TGLabel* _B_entry_label;
        TGLabel* _R_entry_label;
        TGLabel* _r_entry_label;
        TGLabel* _mag_rotation_entry_label;

        TGIcon* _magnet_description_canvas;

        TGHorizontalFrame* _init_magnet_edit_frame();
        TGHorizontalFrame* _init_magnet_edit_buttons_frame();
        TGHorizontalFrame* _init_mag_rotation_edit_frame();

        public:

        BConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h);

        void AddPressed();
        void SavePressed();
        void DeletePressed();
        void MagnetDoubleClicked(TGFrame *f, Int_t btn);

        void UpdateMagnetList();

        std::string ProduceBConfiguration();
        void SetBConfiguration(const std::string config);
        void AddMagnet(std::string config);
        void AddMagnet(MagnetConfigurationDetails new_magnet);
        void LoadDefaultMagnets();
        void SetMagnetRotation(float degrees);
    };
}

#endif 