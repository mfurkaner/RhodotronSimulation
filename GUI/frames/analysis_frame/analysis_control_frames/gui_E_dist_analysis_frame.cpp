#include "gui_E_dist_analysis_frame.h"

//namespace RhodotronSimulatorGUI::frames::subframes{

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

        // Setup the time slider
        time_slider = new TGHSlider(this, 200);
        time_slider->SetPosition(0);
        time_slider->SetRange(0, 500);


        this->AddFrame(select_time_label, select_time_label_layout);
        this->AddFrame(time_slider, center_x_layout);
        this->AddFrame(active_time, center_x_layout);
    }
    
    void EDistributionControlFrame::Setup(){
        active_time->Connect("Modified()", "AnalysisFrame", (void*)parent, 
                             "EnergyDistributionRequested()");

                time_slider->Connect("PositionChanged(Int_t)", "EDistributionControlFrame", this, 
                             "TimeChanged(Int_t)"); 
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

//}