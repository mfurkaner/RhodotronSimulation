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

    static const Int_t common_padding = 5;

    static TGLayoutHints* center_x_layout = new TGLayoutHints(kLHintsCenterX, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* center_y_layout = new TGLayoutHints(kLHintsCenterY, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* left_layout = new TGLayoutHints(kLHintsLeft, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* right_layout = new TGLayoutHints(kLHintsRight, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* top_layout = new TGLayoutHints(kLHintsTop, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* bottom_layout = new TGLayoutHints(kLHintsBottom, common_padding, common_padding, common_padding, common_padding);
    static TGLayoutHints* top_left_layout = new TGLayoutHints(kLHintsTop | kLHintsLeft, common_padding, common_padding, common_padding, common_padding); 


    #define FONT_BOLD_14 "-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1"
    #define FONT_BOLD_12 "-adobe-helvetica-bold-r-*-*-12-*-*-*-*-*-iso8859-1"

    #define FONT_MEDIUM_14 "-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1"
    #define FONT_MEDIUM_12 "-adobe-helvetica-bold-r-*-*-12-*-*-*-*-*-iso8859-1"



    /// @brief B CONFIGURATION FRAME RESOURCES
    static const std::string B_configuration_title = "- B Field Configuration -";
    static const std::string B_configuration_B_entry_label_text = "B (T)";
    static const std::string B_configuration_R_entry_label_text = "R (m)";
    static const std::string B_configuration_r_entry_label_text = "r (m)";
    static const std::string B_configuration_mag_rot_entry_label_text = "Rotation of each magnet (degree)";

    /// @brief E CONFIGURATION FRAME RESOURCES
    static const std::string E_configuration_title = "- E Field Configuration -";
    static const std::string E_configuration_cavity_desc_title = "Cavity Description";
    static const std::string E_configuration_rf_desc_title = "RF Description";
    static const std::string E_configuration_R1_entry_label_text = "R1 (m)";
    static const std::string E_configuration_R2_entry_label_text = "R2 (m)";
    static const std::string E_configuration_Emax_entry_label_text = "Emax (MV/m2)";
    static const std::string E_configuration_freq_entry_label_text = "f (MHz)";
    static const std::string E_configuration_phase_lag_entry_label_text = "Phase Lag (degree)";
    static const std::string E_configuration_cavity_desc_pic_path = "resources/Cavity_desc.png";
    #define E_CONFIGURATION_CAVITY_DESC_PIC_SIZE 175
    static const std::string E_configuration_rf_desc_pic_path = "resources/RF_desc.png";
    #define E_CONFIGURATION_RF_DESC_PIC_SIZE 200

    /// @brief GUN CONFIGURATION FRAME RESOURCES
    static const std::string gun_configuration_title = "- Gun Configuration -";
    static const std::string gun_configuration_gun_desc_pic_path =      "resources/Gun_desc.png";
    #define GUN_CONFIGURATION_GUN_DESC_PIC_SIZE_X 250
    #define GUN_CONFIGURATION_GUN_DESC_PIC_SIZE_Y 130
    static const std::string gun_configuration_Ein_entry_label_text =   "Ein (MeV)";
    static const std::string gun_configuration_targetE_entry_label_text="Target E(MeV)";
    static const std::string gun_configuration_T_entry_label_text =     "Gun Period - T(ns)";
    static const std::string gun_configuration_tg_entry_label_text =    "Gun Active Time - tg(ns)";
    static const std::string gun_configuration_bnum_entry_label_text =  "# of bunch";
    static const std::string gun_configuration_enum_entry_label_text =  "# of e per bunch";


    /// @brief SIM CONFIGURATION FRAME RESOURCES
    static const std::string sim_configuration_title = "- Simulation Configuration -";
    static const std::string sim_configuration_t0_entry_label_text =    "Start time (ns)";
    static const std::string sim_configuration_tf_entry_label_text =    "End time (ns)";
    static const std::string sim_configuration_dt_entry_label_text =    "Time step size (ns)";
    static const std::string sim_configuration_thnum_entry_label_text = "# of threads";
    static const std::string sim_configuration_multh_label_text =       "Enable multithreading";
    static const std::string sim_configuration_epath_entry_label_text = "E field log path";
    static const std::string sim_configuration_bpath_entry_label_text = "B field log path";
    static const std::string sim_configuration_ppath_entry_label_text = "Particles log path";
    static const std::string sim_configuration_cpath_entry_label_text = "Configuration log path";

    /// @brief DEFAULT CONFIGURATION VALUES
    #define DEFAULT_B_MAG_COUNT 3
        const double DEFAULT_B_MAG_B[DEFAULT_B_MAG_COUNT] {-0.0298, -0.041, -0.055};
        const double DEFAULT_B_MAG_R[DEFAULT_B_MAG_COUNT] {0.15, 0.15, 0.15};
        const double DEFAULT_B_MAG_r[DEFAULT_B_MAG_COUNT] {1.103, 1.1444, 1.163};
    #define DEFAULT_B_MAG_ROT 5.0

    #define DEFAULT_E_R1 0.188
    #define DEFAULT_E_R2 0.753
    #define DEFAULT_E_EMAX 0.96
    #define DEFAULT_E_F 107.5
    #define DEFAULT_E_PL -5

    #define DEFAULT_GUN_EIN 0.04
    #define DEFAULT_GUN_TARGETE 2.0
    #define DEFAULT_GUN_T 9.3
    #define DEFAULT_GUN_TG 1.0
    #define DEFAULT_GUN_BNUM 2
    #define DEFAULT_GUN_ENUM 50

    #define DEFAULT_SIM_T0 0
    #define DEFAULT_SIM_TF 50
    #define DEFAULT_SIM_DT 0.0001
    #define DEFAULT_SIM_THNUM 1
    #define DEFAULT_SIM_MULTH_STATE kButtonDisabled
    #define DEFAULT_SIM_EPATH "xy/rf.txt"
    #define DEFAULT_SIM_BPATH "xy/magnet.txt"
    #define DEFAULT_SIM_PPATH "xy/paths/"
    #define DEFAULT_SIM_CPATH "xy/settings.txt"
}

#endif