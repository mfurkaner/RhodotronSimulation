#include "gui_sweep_frame.h"



namespace RhodotronSimulatorGUI::frames{

    SweepFrame::SweepFrame(const TGWindow* p, UInt_t w, UInt_t h, Analysis::Analyzer* analyzer_, GUISimulationHandler* sim_handler_, data::DataProvider* dataProvider_, ConfigurationFrame* config_frame_) : TGVerticalFrame(p, w, h){
        parent = p;

        analyzer = analyzer_;
        sim_handler = sim_handler_;
        dataProvider = dataProvider_;
        config_frame = config_frame_;

        _sweepSelection = new TGComboBox(this);
        _sweepSelection->Connect("Selected(Int_t)", "RhodotronSimulatorGUI::frames::SweepFrame", this, "SweepTypeSelected(Int_t)");
        _sweepSelection->Resize(200,20);
        _fillSweepSelection();

        _phaselag_control_frame = new subframes::PhaseLagSweepControlFrame(this, 600, 900, analyzer, sim_handler, dataProvider, config_frame);

        this->AddFrame(_sweepSelection, center_x_layout);
        this->AddFrame(_phaselag_control_frame, center_layout);

        active_frame = _phaselag_control_frame;
    }

    void SweepFrame::_fillSweepSelection(){
        _sweepSelection->NewEntry(Sweep_type_phaselag_description_text.c_str());
        _sweepTypes.push_back(PhaseLagSweep);
    }

    void SweepFrame::SweepTypeSelected(Int_t id){
        auto index = _sweepSelection->GetSelected();

        if( index < 1|| index > _sweepTypes.size()){
            return;
        }

        switch (_sweepTypes[index - 1])
        {
            case PhaseLagSweep:{
                _phaselag_control_frame->OnNavigatedTo();
                NavigateTo(_phaselag_control_frame);
                break;
            }
        }
    }

    void SweepFrame::OnNavigatedTo(){
        HideAll();
    }

    void SweepFrame::NavigateTo(TGFrame* child_frame){
        HideAll();
        this->ShowFrame(child_frame);
        this->Layout();

        active_frame = child_frame;
    }

    void SweepFrame::HideAll(){
        this->HideFrame(_phaselag_control_frame);

        this->Layout();
    }
}