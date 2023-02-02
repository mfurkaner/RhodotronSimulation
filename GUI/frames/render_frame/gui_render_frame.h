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
#include "TGNumberEntry.h"
#include "TGSlider.h"

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
#include "../../renderer/gui_renderer.h"
#include "../../analyzer/definitions.h"



namespace RhodotronSimulatorGUI::frames{

    class RenderFrame : public TGVerticalFrame{
        TRootEmbeddedCanvas* canvas;
        TGHProgressBar* progressBar;
        const TGWindow* parent;
        TGNumberEntry* active_time;
        TGSlider* time_slider;
        renderer::Renderer* renderer;

        float _time = 0;
        float _endtime = 40;
        
    public:
        RenderFrame(const TGWindow* p, UInt_t w, UInt_t h, renderer::Renderer* renderer_);

        void SetTimeInterval(float starttime, float endtime);
        void SetTargetEnergy(float targetEn){ renderer->SetTargetEnergy(targetEn);}
        void SetR1R2(float r1, float r2);

        void UpdateProgressBar(uint8_t status);
        TGProgressBar* GetProgressBar();

        void TimeChanged();
        void SliderPositionChanged(int pos);

        void SetupRenderer();
        
        void SavePressed();
        void PlayPressed();

    };

}



#endif