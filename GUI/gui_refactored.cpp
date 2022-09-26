#include <stdio.h>
#include <string.h>
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
#include "TASImage.h"


#include "comm/signal.h"
#include "frames/gui_main_frame.h"
#include "frames/gui_main_frame.cpp"
#include "frames/subframes/gui_main_buttons.cpp"
#include "frames/subframes/gui_config_frame.cpp"
#include "frames/subframes/gui_render_frame.cpp"
#include "comm/sim_server.cpp"




void gui_refactored(){
    gSystem->cd("..");
    //gSystem->DispatchOneEvent();
    gSystem->ProcessEvents();
    new RhodotronSimulatorGUI::frames::MainFrame( gClient->GetRoot(), MAIN_FRAME_W, MAIN_FRAME_W);
}