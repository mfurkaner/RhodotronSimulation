#ifndef GUI_ANALYSIS_E_DIST_H
#define GUI_ANALYSIS_E_DIST_H

#include "TGNumberEntry.h"
#include "TGSlider.h"


#include "../../gui_frames_common.h"

//namespace RhodotronSimulatorGUI::frames::subframes{

    class EDistributionControlFrame : public TGVerticalFrame{
        const TGWindow* parent;

        TGLabel* select_time_label;
        TGNumberEntry* active_time;
        TGSlider* time_slider;

        float _starttime = 0;
        float _endtime = 40;
        
    public:
        EDistributionControlFrame(const TGWindow* p, UInt_t w, UInt_t h);

        void Setup();

        void SetTimeInterval(float starttime, float endtime);
        void TimeChanged(Int_t pos);
        double GetRequestedTime();
    };

//}

#endif