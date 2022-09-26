#ifndef GUI_RENDER_FRAME_H
#define GUI_RENDER_FRAME_H


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

    class RenderFrame : public TGVerticalFrame{
        TRootEmbeddedCanvas* canvas;
        TGHProgressBar* progressBar;
        const TGWindow* parent;
        
    public:
        RenderFrame(const TGWindow* p, UInt_t w, UInt_t h);


        void UpdateProgressBar(uint8_t status);
        TGProgressBar* GetProgressBar();
    };

}



#endif