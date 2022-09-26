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


    #define MAIN_FRAME_W 500
    #define MAIN_FRAME_H 500

    #define MAIN_BUTTON_FRAME_W 500
    #define MAIN_BUTTON_FRAME_H 500

    #define CONFIG_FRAME_W 500
    #define CONFIG_FRAME_H 500
    #define CONFIG_FRAME_LINE_W 150
    #define CONFIG_FRAME_LINE_H 50

    static const Int_t common_padding = 2;

    static TGLayoutHints* center_layout = new TGLayoutHints(kLHintsCenterX, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* left_layout = new TGLayoutHints(kLHintsLeft, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* right_layout = new TGLayoutHints(kLHintsRight, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* top_layout = new TGLayoutHints(kLHintsTop, common_padding, common_padding, common_padding, common_padding); 
    static TGLayoutHints* bottom_layout = new TGLayoutHints(kLHintsBottom, common_padding, common_padding, common_padding, common_padding); 




}

#endif