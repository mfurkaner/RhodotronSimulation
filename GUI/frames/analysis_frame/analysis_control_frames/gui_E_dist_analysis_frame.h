#ifndef GUI_ANALYSIS_E_DIST_H
#define GUI_ANALYSIS_E_DIST_H

#include "TGNumberEntry.h"
#include "TGSlider.h"


#include "../../gui_frames_common.h"

namespace RhodotronSimulatorGUI::frames::subframes{

    class EDistributionControlFrame : public TGVerticalFrame{
        const TGWindow* parent;

        TGLabel* select_time_label;
        TGNumberEntry* active_time;
        TGSlider* time_slider;

        float _starttime = 0;
        float _endtime = 40;
        
    public:
        EDistributionControlFrame(const TGWindow* p, UInt_t w, UInt_t h);

        void SetTimeInterval(float starttime, float endtime);
        void TimeChanged(Int_t pos);
        double GetRequestedTime();
    };

    EDistributionControlFrame::EDistributionControlFrame(const TGWindow* p, UInt_t w, UInt_t h)
        : TGVerticalFrame(p, w, h){
        
        parent = p;

        select_time_label = new TGLabel(this, Analysis_select_time_label_text.c_str());
        select_time_label->SetTextFont(FONT_BOLD_12);
        auto select_time_label_layout = new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0);

        // Setup the time entry field
        active_time = new TGNumberEntry(this,0.0,3, -1, TGNumberFormat::kNESRealOne, 
                                TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0.1, 50);
        active_time->Resize(100, 25);
        active_time->Connect("Modified()", "RhodotronSimulatorGUI::frames::AnalysisFrame", (void*)parent, 
                             "EnergyDistributionRequested()");

        // Setup the time slider
        time_slider = new TGHSlider(this, 200);
        time_slider->SetPosition(0);
        time_slider->SetRange(0, 500);
        time_slider->Connect("PositionChanged(Int_t)", "RhodotronSimulatorGUI::frames::subframes::EDistributionControlFrame", this, 
                             "TimeChanged(Int_t)");

        this->AddFrame(select_time_label, select_time_label_layout);
        this->AddFrame(time_slider, center_x_layout);
        this->AddFrame(active_time, center_x_layout);
    }

    void EDistributionControlFrame::SetTimeInterval(float starttime, float endtime){
        _starttime = starttime;
        _endtime = endtime;
        if ( endtime <= 0.1 )
            endtime = 0;

        time_slider->SetRange(starttime * 10, endtime * 10);
        active_time->SetLimitValues(starttime, endtime);
        active_time->SetNumber(starttime, false);
        time_slider->SetPosition(starttime * 10);
    }

    void EDistributionControlFrame::TimeChanged(Int_t pos){
        float time = pos/10.0;
        if ( active_time->GetNumber() != time ){
            active_time->SetNumber(time);
            active_time->Modified();
        }
    }

    double EDistributionControlFrame::GetRequestedTime(){
        if (active_time == nullptr){
            return -1;
        }
        return active_time->GetNumber();
    }
}

#endif