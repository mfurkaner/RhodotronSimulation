#ifndef GUI_MAIN_BUTTONS_FRAME_H
#define GUI_MAIN_BUTTONS_FRAME_H


#include "TSystem.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGTextView.h"
#include "TGLabel.h"
#include "TGTextEntry.h"
#include "TGLViewer.h"
#include "TRootEmbeddedCanvas.h"
#include "TGButtonGroup.h"
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

#include "../gui_frames_common.h"



namespace RhodotronSimulatorGUI::frames{

    class MainButtonsFrame : public TGHorizontalFrame{
        std::vector<TGTextButton*> buttons;
        const TGWindow* parent;
        
    public:
        static const std::vector<std::string> button_names;
        static const std::vector<std::string> button_functions;

        MainButtonsFrame(const TGWindow* p, UInt_t w, UInt_t h);

        void HideByName(const std::string& name);
        void ShowByName(const std::string& name);
        void DisableByName(const std::string& name);
        void EnableByName(const std::string& name, bool enable = true);
        void EnableAll();
    };


}



#endif