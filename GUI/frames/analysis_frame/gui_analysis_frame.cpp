#include "gui_analysis_frame.h"
#include "TGNumberEntry.h"
#include "TColor.h"

//namespace RhodotronSimulatorGUI::frames{


    AnalysisFrame::AnalysisFrame(const TGWindow* p, UInt_t w, UInt_t h, Analyzer* analyzer_) 
        : TGVerticalFrame(p, w, h){
        parent = p;
        analyzer = analyzer_;

        //auto color = TColor::GetColor(232, 232, 233);
        canvas = new TRootEmbeddedCanvas("output", this, 500, 500);


        _Edist_control_frame = new EDistributionControlFrame(this, 300, 300);
        _tvsE_control_frame = new TvsEControlFrame(this, 300, 300);


        _analysisSelection = new TGComboBox(this);

        _analysisSelection->Resize(200,20);
        _fillAnalysisSelection();


        _save_button = new TGTextButton(this, "Save");

        this->AddFrame(_analysisSelection, center_x_layout);
        this->AddFrame(canvas, center_x_layout);
        
        this->AddFrame(_Edist_control_frame, center_x_layout);
        this->AddFrame(_tvsE_control_frame, center_x_layout);

        this->AddFrame(_save_button, center_x_layout);

        active_frame = _Edist_control_frame;
    }

    void AnalysisFrame::Setup(){
        _analysisSelection->Connect("Selected(Int_t)", "AnalysisFrame", this, "AnalysisTypeSelected(Int_t)");
        _save_button->Connect("Clicked()", "AnalysisFrame", this, "SavePressed()");
        
        _Edist_control_frame->Setup();
        _tvsE_control_frame->Setup();
    }

    void AnalysisFrame::_fillAnalysisSelection(){
        _analysisSelection->NewEntry(Analysis_type_energy_distribution_description_text.c_str());
        _analysisTypes.push_back(EnergyDistribution);
        _analysisSelection->NewEntry(Analysis_type_sing_e_TvsE_description_text.c_str());
        _analysisTypes.push_back(single_e_t_vs_E);
    }

    void AnalysisFrame::SetTimeInterval(float starttime, float endtime){
        _starttime = starttime;
        _endtime = endtime;

        _Edist_control_frame->SetTimeInterval(starttime, endtime);
        _tvsE_control_frame->SetTimeInterval(starttime, endtime);
    }

    void AnalysisFrame::SetEnumBnum(int _enum_, int _bnum_){
        _enum = _enum_;
        _bnum = _bnum_;
        _tvsE_control_frame->SetBnum(_bnum);
        _tvsE_control_frame->SetEnum(_enum);
    }

    void AnalysisFrame::SetupAnalyzer(){
        analyzer->SetCanvas(canvas);
    }

    void AnalysisFrame::AnalysisTypeSelected(Int_t id){
        auto index = _analysisSelection->GetSelected();

        if( index < 1|| index > _analysisTypes.size()){
            return;
        }

        switch (_analysisTypes[index - 1])
        {
            case EnergyDistribution:{
                NavigateTo(_Edist_control_frame);
                EnergyDistributionRequested();
                break;
            }

            case single_e_t_vs_E:{
                NavigateTo(_tvsE_control_frame);
                TvsERequested();
                break;
            }
        }
    }


    void AnalysisFrame::EnergyDistributionRequested(){
        analyzer->DrawEnergyDistribution(_Edist_control_frame->GetRequestedTime());
    }

    void AnalysisFrame::TvsERequested(){
        int e_index = _tvsE_control_frame->GetEIndex();
        int b_index = _tvsE_control_frame->GetBIndex();

        auto times = _tvsE_control_frame->GetTimeIntervalToDraw();
        analyzer->DrawEvsT(b_index, e_index, times.first, times.second);
    }

    void AnalysisFrame::SavePressed(){
        canvas->SaveAs("analysis.png");
    }

    void AnalysisFrame::clear(){
        canvas->Clear();
    }

    void AnalysisFrame::OnNavigatedTo(){
        HideAll(true);
    }

    void AnalysisFrame::NavigateTo(TGFrame* child_frame){
        HideAll();
        this->ShowFrame(canvas);
        this->ShowFrame(child_frame);

        active_frame = child_frame;
    }

    void AnalysisFrame::HideAll(bool hide_canvas){
        if (hide_canvas) this->HideFrame(canvas);
        this->HideFrame(_Edist_control_frame);
        this->HideFrame(_tvsE_control_frame);
    }
//}
