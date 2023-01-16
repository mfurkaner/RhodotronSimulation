#include "gui_render_frame.h"
#include "TGNumberEntry.h"


namespace RhodotronSimulatorGUI::frames{


    RenderFrame::RenderFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGVerticalFrame(p, w, h){
        parent = p;

        // Setup the progressbar
        progressBar = new TGHProgressBar(this, 250, 50);
        progressBar->SetMin(0);
        progressBar->SetMax(0b00011111);
        progressBar->ShowPosition();
        progressBar->Activate(false);

        canvas = new TRootEmbeddedCanvas("output", this, 500, 500);

        // Setup the play button
        auto play_button = new TGTextButton(this, "Play");
        play_button->Connect("Clicked()", "RhodotronSimulatorGUI::renderer::Renderer", &renderer, 
                             "RunRendered()");

        // Setup the time entry field
        active_time = new TGNumberEntry(this,0.0,3, -1, TGNumberFormat::kNESRealOne, 
                                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0, 29.9);
        active_time->Resize(100, 25);
        active_time->Connect("Modified()", "RhodotronSimulatorGUI::frames::RenderFrame", this, 
                             "TimeChanged()");

        // Setup the time slider
        time_slider = new TGHSlider(this, 200);
        time_slider->SetPosition(0);
        time_slider->SetRange(0, 300);
        time_slider->Connect("PositionChanged(Int_t)", "RhodotronSimulatorGUI::frames::RenderFrame", this, 
                             "SliderPositionChanged(Int_t)");


        this->AddFrame(progressBar, center_layout);
        this->AddFrame(canvas, center_layout);
        this->AddFrame(play_button, center_layout);
        this->AddFrame(time_slider, center_layout);
        this->AddFrame(active_time, center_layout);
    }

    void RenderFrame::UpdateProgressBar(uint8_t progress){
        if ( progressBar->IsActive() == false){
            this->ShowFrame(progressBar);
            progressBar->Activate(true);
        }
        progressBar->SetPosition(progress);
    }

    TGProgressBar* RenderFrame::GetProgressBar() { return progressBar; }
        

    void RenderFrame::TimeChanged(){
        double time = active_time->GetNumber();
        renderer.GoToTime(time);
        if ( time_slider->GetPosition()/10.0 != time ){
            time_slider->SetPosition(time * 10);
        }
    }

    void RenderFrame::SliderPositionChanged(int pos){
        double time = pos/10.0;
        if ( active_time->GetNumber() != time ){
            active_time->SetNumber(time);
            active_time->Modified();
        }
    };


    void RenderFrame::Render(){
        this->HideFrame(progressBar);
        progressBar->Activate(false);
        renderer.fillLogs();
        renderer.Render(canvas);
    }


}
