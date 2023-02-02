#ifndef GUI_t_vs_E_DIST_H
#define GUI_t_vs_E_DIST_H

#include "TGNumberEntry.h"
#include "TGDoubleSlider.h"


#include "../../gui_frames_common.h"

namespace RhodotronSimulatorGUI::frames::subframes{

    class TvsEControlFrame : public TGVerticalFrame{
        const TGWindow* parent;

        TGLabel* select_time_label;
        TGLabel* e_label;
        TGLabel* b_label;
        TGNumberEntry* e_index;
        TGNumberEntry* b_index;

        TGDoubleHSlider* time_interval_select;

        double _start_time;
        double _end_time;
        
    public:
        TvsEControlFrame(const TGWindow* p, UInt_t w, UInt_t h);
        void SetTimeInterval(double start_time, double end_time);
        void SetEnum(int _enum);
        void SetBnum(int bnum);
        int GetBIndex();
        int GetEIndex();
        std::pair<double,double> GetTimeIntervalToDraw();
    };

    TvsEControlFrame::TvsEControlFrame(const TGWindow* p, UInt_t w, UInt_t h)
        : TGVerticalFrame(p, w, h){
        
        parent = p;

        select_time_label = new TGLabel(this, Analysis_select_time_label_text.c_str());
        select_time_label->SetTextFont(FONT_BOLD_12);
        auto select_time_label_layout = new TGLayoutHints(kLHintsCenterX, 0, 0, 10, 0);


        time_interval_select = new TGDoubleHSlider(this, 300);
        time_interval_select->SetRange(0.0, 50.0);
        time_interval_select->SetPosition(0.0,50.0);

        auto b_e_label_frame = new TGHorizontalFrame(this);
        b_label = new TGLabel(b_e_label_frame, Analysis_tvsE_b_index_label_text.c_str());
        e_label = new TGLabel(b_e_label_frame, Analysis_tvsE_e_index_label_text.c_str());
        
        auto b_label_layout = new TGLayoutHints(kLHintsCenterX, 0, 50, 5, 0);
        auto e_label_layout = new TGLayoutHints(kLHintsCenterX, 10, 0, 5, 0);

        b_e_label_frame->AddFrame(b_label, b_label_layout);
        b_e_label_frame->AddFrame(e_label, e_label_layout);

        auto b_e_input_frame = new TGHorizontalFrame(this);
        // Setup the time entry field
        b_index = new TGNumberEntry(b_e_input_frame,1,3, -1, TGNumberFormat::kNESInteger, 
                    TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 1, 5);
        b_index->Resize(100, 25);
        b_index->Connect("Modified()", "RhodotronSimulatorGUI::frames::AnalysisFrame", (void*)parent, 
                             "TvsERequested()");

        e_index = new TGNumberEntry(b_e_input_frame,1,3, -1, TGNumberFormat::kNESInteger, 
                    TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 1, 5);
        e_index->Resize(100, 25);
        e_index->Connect("Modified()", "RhodotronSimulatorGUI::frames::AnalysisFrame", (void*)parent, 
                             "TvsERequested()");

        auto b_e_input_layout = new TGLayoutHints(kLHintsCenterX, 10, 10, 0, 10);

        b_e_input_frame->AddFrame(b_index, b_e_input_layout);
        b_e_input_frame->AddFrame(e_index, b_e_input_layout);

        this->AddFrame(select_time_label, select_time_label_layout);
        this->AddFrame(time_interval_select, center_x_layout);
        this->AddFrame(b_e_label_frame, center_x_layout);
        this->AddFrame(b_e_input_frame, b_e_input_layout);
    }
    
    void TvsEControlFrame::SetTimeInterval(double start_time, double end_time){
        _start_time = start_time;
        _end_time = end_time;

        time_interval_select->SetRange(start_time, end_time);
        time_interval_select->SetPosition(start_time, end_time);
        time_interval_select->Connect("PositionChanged()","RhodotronSimulatorGUI::frames::AnalysisFrame", (void*)parent, "TvsERequested()");
    }

    void TvsEControlFrame::SetEnum(int _enum){
        e_index->SetLimitValues(1, _enum);
    }

    void TvsEControlFrame::SetBnum(int bnum){
        b_index->SetLimitValues(1, bnum);
    }

    int TvsEControlFrame::GetBIndex(){
        return (b_index  ? b_index->GetNumber() : 1);
    }

    int TvsEControlFrame::GetEIndex(){
        return (e_index ? e_index->GetNumber() : 1);
    }

    std::pair<double,double> TvsEControlFrame::GetTimeIntervalToDraw(){
        std::pair<double,double> result;
        result.first = time_interval_select->GetMinPosition();
        result.second = time_interval_select->GetMaxPosition();
        return result;
    }
}

#endif