#include "gui_render_frame.h"
#include "TGNumberEntry.h"
#include "TColor.h"

namespace RhodotronSimulatorGUI::frames{


    RenderFrame::RenderFrame(const TGWindow* p, UInt_t w, UInt_t h) 
        : TGVerticalFrame(p, w, h){
        parent = p;

        // Setup the progressbar
        progressBar = new TGHProgressBar(this, 250, 50);
        progressBar->SetMin(0);
        progressBar->SetMax(0b00011111);
        progressBar->ShowPosition();
        progressBar->Activate(false);

        //auto color = TColor::GetColor(232, 232, 233);
        canvas = new TRootEmbeddedCanvas("output", this, 500, 500);

        // Setup the play button
        auto play_button = new TGTextButton(this, "Play");
        play_button->Connect("Clicked()", "RhodotronSimulatorGUI::frames::RenderFrame", this, //"RhodotronSimulatorGUI::renderer::Renderer", &renderer, 
                             "PlayPressed()");

        auto save_button = new TGTextButton(this, "Save");
        save_button->Connect("Clicked()", "RhodotronSimulatorGUI::frames::RenderFrame", this, //"RhodotronSimulatorGUI::renderer::Renderer", &renderer, 
                             "SavePressed()");

        // Setup the time entry field
        active_time = new TGNumberEntry(this,0.0,3, -1, TGNumberFormat::kNESRealOne, 
                                TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0.1, 49.9);
        active_time->Resize(100, 25);
        active_time->Connect("Modified()", "RhodotronSimulatorGUI::frames::RenderFrame", this, 
                             "TimeChanged()");

        // Setup the time slider
        time_slider = new TGHSlider(this, 200);
        time_slider->SetPosition(0);
        time_slider->SetRange(0, 499);
        time_slider->Connect("PositionChanged(Int_t)", "RhodotronSimulatorGUI::frames::RenderFrame", this, 
                             "SliderPositionChanged(Int_t)");


        this->AddFrame(progressBar, center_x_layout);
        this->AddFrame(canvas, center_x_layout);
        this->AddFrame(play_button, center_x_layout);
        this->AddFrame(time_slider, center_x_layout);
        this->AddFrame(active_time, center_x_layout);
        this->AddFrame(save_button, center_x_layout);

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
        float time = active_time->GetNumber();
        renderer.GoToTime(time);
        if ( time_slider->GetPosition()/10.0 != time ){
            time_slider->SetPosition(time * 10);
        }
    }

    void RenderFrame::SliderPositionChanged(int pos){
        float time = pos/10.0;
        if ( active_time->GetNumber() != time ){
            active_time->SetNumber(time);
            active_time->Modified();
        }
    }

    void RenderFrame::SetTimeInterval(float starttime, float endtime){
        std::cout << "endtime : " << endtime << std::endl;
        if ( endtime <= 0.1 )
            endtime = 0;
        else{
            endtime -= 0.1;
        }

        std::cout << "Setting time interval to " << starttime << " : " << endtime << std::endl; 
        time_slider->SetRange(starttime * 10, endtime * 10);
        active_time->SetLimitValues(starttime, endtime);
        active_time->SetNumber(starttime);
        time_slider->SetPosition(starttime * 10);
        active_time->Modified();
        std::cout << "Ended to setting time interval" << std::endl; 
    }

    void RenderFrame::SetR1R2(float r1, float r2){
        canvas->GetCanvas()->Range(-r2*2, -r2*2, r2*2, r2*2);
    }

    void RenderFrame::SavePressed(){
        renderer.SaveGif();
    }

    void RenderFrame::PlayPressed(){
        renderer.RunRendered();
    }


    void RenderFrame::Render(){
        this->HideFrame(progressBar);
        progressBar->Activate(false);
        renderer.fillLogs();
        renderer.Render(canvas);
    }


}
