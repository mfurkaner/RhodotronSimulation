#include "gui_run_frame.h"
#include "../gui_main_frame.h"

namespace RhodotronSimulatorGUI::frames{


    RunFrame::RunFrame(const TGWindow* p, UInt_t w, UInt_t h) 
        : TGVerticalFrame(p, w, h){
        parent = p;

        _label = new TGLabel(this, Run_frame_title.c_str());
        _label->SetTextFont(FONT_BOLD_14);

        _status = new TGLabel(this, Run_frame_status_title_not_running.c_str());
        _status->SetTextFont(FONT_MEDIUM_12);


        // Setup the progressbar
        progressBar = new TGHProgressBar(this, TGProgressBar::EBarType::kFancy, 250);
        progressBar->SetHeight(100);
        progressBar->SetMin(0);
        progressBar->SetMax(0b00011111);
        progressBar->ShowPosition();
        progressBar->SetBarColor("lightblue");


        auto button_frame = new TGHorizontalFrame(this);
        // Setup the play button
        _runButton = new TGTextButton(button_frame, "Run Simulation");
        _runButton->Connect("Clicked()", "RhodotronSimulatorGUI::frames::RunFrame", this, //"RhodotronSimulatorGUI::renderer::Renderer", &renderer, 
                             "RunPressed()");

        _stopButton = new TGTextButton(button_frame, "Stop Simulation");
        _stopButton->Connect("Clicked()", "RhodotronSimulatorGUI::frames::RunFrame", this, //"RhodotronSimulatorGUI::renderer::Renderer", &renderer, 
                             "StopPressed()");

        button_frame->AddFrame(_runButton, center_y_layout);
        button_frame->AddFrame(_stopButton, center_y_layout);

        this->AddFrame(_label, center_x_layout);
        this->AddFrame(_status, center_x_layout);
        this->AddFrame(progressBar, center_x_layout);
        this->AddFrame(button_frame, center_x_layout);

        _stopButton->SetEnabled(false);
    }

    void RunFrame::RunPressed(){
        sim_handler->set_progress_bar(progressBar);
        sim_handler->set_status_label(_status);
        sim_handler->spawn_server();
        sim_handler->spawn_simulation();
        _stopButton->SetEnabled(true);

        ((MainFrame*)parent)->ClearData();
    }

    void RunFrame::StopPressed(){
        // TODO : THIS DOES NOT WORK
        sim_handler->kill_simulation();
        sim_handler->kill_server();
        _stopButton->SetEnabled(false);
    }

}
