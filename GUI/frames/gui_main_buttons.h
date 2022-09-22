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


#ifndef GUI_MAIN_BUTTONS_FRAME_H
#define GUI_MAIN_BUTTONS_FRAME_H



namespace RhodotronSimulatorGUI::frames{



    class MainButtonsFrame : public TGHorizontalFrame{
        std::vector<TGTextButton*> buttons;
        

        public:
        static const std::vector<std::string> button_names;




    };



}



#endif