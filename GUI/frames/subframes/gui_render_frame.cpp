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
        play_button->Connect("Clicked()", "RhodotronSimulatorGUI::renderer::Renderer", &renderer, "run_rendered()");

        // Setup the go button
        auto go_button = new TGTextButton(this, "Go");
        go_button->Connect("Clicked()", "RhodotronSimulatorGUI::frames::RenderFrame", this, "GoClicked()");

        // Setup the time entry
        go_to_time = new TGNumberEntry(this,0.0,3, -1, TGNumberFormat::kNESRealOne, 
                                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0, 29.9);
        go_to_time->Resize(100, 25);


        time_slider = new TGDoubleHSlider(this, 200);
        time_slider->SetPosition(0.0, 1.0);

        std::cout << "Time slider min : " << time_slider->GetMinPosition() << " max : " << time_slider->GetMaxPosition() << std::endl;
        time_slider->SetRange(0.0, 30.9);
        time_slider->Connect("PositionChanged()", "RhodotronSimulatorGUI::frames::RenderFrame", this, "SliderPositionChanged()");

        time_slider->Draw();


        this->AddFrame(progressBar, center_layout);
        this->AddFrame(canvas, center_layout);
        this->AddFrame(play_button, center_layout);
        this->AddFrame(time_slider, center_layout);
        this->AddFrame(go_to_time, center_layout);
        this->AddFrame(go_button, center_layout);

    }

    void RenderFrame::UpdateProgressBar(uint8_t progress){
        if ( progressBar->IsActive() == false){
            this->ShowFrame(progressBar);
            progressBar->Activate(true);
        }
        progressBar->SetPosition(progress);
    }

    TGProgressBar* RenderFrame::GetProgressBar(){
        return progressBar;
    }

    void RenderFrame::GoClicked(){
        renderer.GoToTime(go_to_time->GetNumber());
    }

    void RenderFrame::SliderPositionChanged(){
        double time, dummy;
        time_slider->GetPosition(&time, &dummy);

        // Filter the numbers smaller than the step size
        time = ((int)(time*10))/10.0;
        renderer.GoToTime(time);
    };

/*
    void RenderFrame::GetGif(std::string filename){
        //dummy_image
        TImage* image;
        uint32_t i = 0;
        while ( true ) {
            if ( i == 10 ) break;
            std::string name = filename + ".";
            name += std::to_string(i);
            image = TImage::Open(name.c_str());
            //TASImage* image = new TASImage(
            std::cout << name << " is added" << std::endl;
            images.push_back(image);
            i++;
        }
        std::cout << "There are " << images.size() << " images."<< std::endl;
    }


    void RenderFrame::DrawGif(){
        

        for (uint32_t i = 0; i < images.size(); i++) {
            std::cout << "drawing " << i << std::endl;
            canvas->GetCanvas();
            images[i]->Draw();
            canvas->GetCanvas()->Modified();
            //canvas->GetCanvas()->cd(1)->Update();
            canvas->GetCanvas()->Update();
            sleep(1);
        }
    }*/


    void RenderFrame::Render(){
        this->HideFrame(progressBar);
        progressBar->Activate(false);
        renderer.fillLogs();
        renderer.render(canvas);
    }



}
