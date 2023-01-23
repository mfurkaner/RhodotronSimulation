#ifndef GUI_FRAMES_COMMON_H
#define GUI_FRAMES_COMMON_H

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


namespace RhodotronSimulatorGUI::frames{


    #define MAIN_FRAME_W 800
    #define MAIN_FRAME_H 800

    #define MAIN_BUTTON_FRAME_W 500
    #define MAIN_BUTTON_FRAME_H 500

    #define CONFIG_FRAME_W 500
    #define CONFIG_FRAME_H 500
    #define CONFIG_FRAME_LINE_W 150
    #define CONFIG_FRAME_LINE_H 50
    #define CONFIG_FRAME_B_FRAME_W 250
    #define CONFIG_FRAME_B_FRAME_H 250

    #define RENDER_FRAME_W 500
    #define RENDER_FRAME_H 500

    static const Int_t common_padding = 2;

    static TGLayoutHints* center_layout = new TGLayoutHints(kLHintsCenterX, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* left_layout = new TGLayoutHints(kLHintsLeft, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* right_layout = new TGLayoutHints(kLHintsRight, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* top_layout = new TGLayoutHints(kLHintsTop, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* bottom_layout = new TGLayoutHints(kLHintsBottom, common_padding, common_padding, common_padding, common_padding);
    static TGLayoutHints* top_left_layout = new TGLayoutHints(kLHintsTop | kLHintsLeft, common_padding, common_padding, common_padding, common_padding); 


    static const std::string B_configuration_B_entry_label_text = "B (T)";
    static const std::string B_configuration_R_entry_label_text = "R (m)";
    static const std::string B_configuration_r_entry_label_text = "r (m)";
    static const std::string B_configuration_mag_rot_entry_label_text = "Rotation of each magnet (degree)";


    static const std::string E_configuration_R1_entry_label_text = "R1 (m)";
    static const std::string E_configuration_R2_entry_label_text = "R2 (m)";
    static const std::string E_configuration_Emax_entry_label_text = "Emax (MV/m2)";
    static const std::string E_configuration_freq_entry_label_text = "f (MHz)";
    static const std::string E_configuration_phase_lag_entry_label_text = "Phase Lag (degree)";
    static const std::string E_configuration_cavity_desc_pic_path = "resources/Cavity-1.png";
    #define E_CONFIGURATION_CAVITY_DESC_PIC_SIZE 250
    static const std::string E_configuration_rf_desc_pic_path = "resources/RF-1.png";
    #define E_CONFIGURATION_RF_DESC_PIC_SIZE 300

}

#endif