#include "gui_run_frame.h"

//namespace RhodotronSimulatorGUI::frames{


    RunFrame::RunFrame(const TGWindow* p, UInt_t w, UInt_t h) 
        : TGVerticalFrame(p, w, h){
        parent = p;

        _label = new TGLabel(this, Run_frame_title.c_str());
        _label->SetTextFont(FONT_BOLD_14);

        _status = new TGLabel(this, Run_frame_status_title_not_running.c_str());
        _status->SetTextFont(FONT_MEDIUM_12);
        _status_mutex = std::make_shared<std::mutex>();


        // Setup the progressbar
        progressBar = new TGHProgressBar(this, TGProgressBar::EBarType::kFancy, 250);
        progressBar->SetHeight(100);
        progressBar->SetMin(0);
        progressBar->SetMax(0b00011111);
        progressBar->ShowPosition();
        progressBar->SetBarColor("lightblue");
        _progressBar_mutex = std::make_shared<std::mutex>();


        auto button_frame = new TGHorizontalFrame(this);
        // Setup the play button
        _runButton = new TGTextButton(button_frame, "Run Simulation");


        _pauseButton = new TGTextButton(button_frame, "Pause Simulation");


        _continueButton = new TGTextButton(button_frame, "Continue Simulation");


        _stopButton = new TGTextButton(button_frame, "Stop Simulation");

        button_frame->AddFrame(_runButton, center_y_layout);
        button_frame->AddFrame(_pauseButton, center_y_layout); 
        button_frame->AddFrame(_continueButton, center_y_layout);
        button_frame->AddFrame(_stopButton, center_y_layout);

        this->AddFrame(_label, center_x_layout);
        this->AddFrame(_status, center_x_layout);
        this->AddFrame(progressBar, center_x_layout);
        this->AddFrame(button_frame, center_x_layout);

        ShowStandbyButtons();
    }

    void RunFrame::Setup(){
        _runButton->Connect("Clicked()", "RunFrame", this, //"RhodotronSimulatorGUI::renderer::Renderer", &renderer, 
                             "RunPressed()");
        _pauseButton->Connect("Clicked()", "RunFrame", this, //"RhodotronSimulatorGUI::renderer::Renderer", &renderer, 
                             "PausePressed()");
        _continueButton->Connect("Clicked()", "RunFrame", this, //"RhodotronSimulatorGUI::renderer::Renderer", &renderer, 
                             "ContinuePressed()");
        _stopButton->Connect("Clicked()", "RunFrame", this, //"RhodotronSimulatorGUI::renderer::Renderer", &renderer, 
                             "StopPressed()");
    }

    void RunFrame::RunPressed(){
        sim_handler->set_progress_bar(progressBar, _progressBar_mutex);
        sim_handler->set_status_label(_status, _status_mutex);

        ShowRunningButtons();

        sim_handler->spawn_server();
        sim_handler->spawn_simulation();

        dp->clearLogs();
        renderer->clear();

        if(_runCheckerThread != NULL){
            if(_runCheckerThread->joinable()){
                _runCheckerThread->join();
            }
            delete _runCheckerThread;
        }
        _runCheckerThread = new std::thread(RunFrame::RunCheckerThreadWork, sim_handler, this);
    }


    void RunFrame::PausePressed(){
        sim_handler->pause_simulation();

        _progressBar_mutex->lock();
        _status_mutex->lock();
        //((TGCompositeFrame*)_pauseButton->GetParent())->HideFrame(_pauseButton);
        _pauseButton->SetEnabled(false);
        //((TGCompositeFrame*)_continueButton->GetParent())->HideFrame(_continueButton);
        _continueButton->SetEnabled(true);
        //Layout();
        _status_mutex->unlock();
        _progressBar_mutex->unlock();
    }

    void RunFrame::ContinuePressed(){
        sim_handler->continue_simulation();

        _progressBar_mutex->lock();
        _status_mutex->lock();
        //((TGCompositeFrame*)_pauseButton->GetParent())->HideFrame(_pauseButton);
        _pauseButton->SetEnabled(true);
        //((TGCompositeFrame*)_continueButton->GetParent())->HideFrame(_continueButton);
        _continueButton->SetEnabled(false);
        //Layout();
        _status_mutex->unlock();
        _progressBar_mutex->unlock();
    }

    void RunFrame::StopPressed(){
        auto mboxHandler = GUIMessageBoxHandler::GetObject();

        auto sure = mboxHandler->DrawYesNoExclamation(
            Run_frame_mbox_title_stopping.c_str(), Run_frame_mbox_msg_stopping.c_str());
            
        if(sure == Yes){
            sim_handler->kill_simulation();
            sim_handler->kill_server();
            
            ShowStandbyButtons();
        }
    }

    void RunFrame::ShowRunningButtons(){
        _progressBar_mutex->lock();
        _status_mutex->lock();
        //((TGCompositeFrame*)_runButton->GetParent())->ShowFrame(_runButton);
        _runButton->SetEnabled(false);
        //((TGCompositeFrame*)_stopButton->GetParent())->HideFrame(_stopButton);        
        _stopButton->SetEnabled(true);
        //((TGCompositeFrame*)_pauseButton->GetParent())->HideFrame(_pauseButton);
        _pauseButton->SetEnabled(true);
        //((TGCompositeFrame*)_continueButton->GetParent())->HideFrame(_continueButton);
        _continueButton->SetEnabled(false);
        //((TGCompositeFrame*)_runButton->GetParent())->Layout();
        //Layout();
        _status_mutex->unlock();
        _progressBar_mutex->unlock();
    }

    void RunFrame::ShowStandbyButtons(){
        _progressBar_mutex->lock();
        _status_mutex->lock();
        //((TGCompositeFrame*)_runButton->GetParent())->ShowFrame(_runButton);
        _runButton->SetEnabled(true);
        //((TGCompositeFrame*)_stopButton->GetParent())->HideFrame(_stopButton);        
        _stopButton->SetEnabled(false);
        //((TGCompositeFrame*)_pauseButton->GetParent())->HideFrame(_pauseButton);
        _pauseButton->SetEnabled(false);
        //((TGCompositeFrame*)_continueButton->GetParent())->HideFrame(_continueButton);
        _continueButton->SetEnabled(false);
        //((TGCompositeFrame*)_runButton->GetParent())->Layout();
        //Layout();
        _status_mutex->unlock();
        _progressBar_mutex->unlock();
    }

    void RunFrame::RunFinished(){
        ShowStandbyButtons();
        dp->fillLogs();
    }

    void RunFrame::RunCheckerThreadWork(GUISimulationHandler* _sim_handler, RunFrame* _frame){
        while(_sim_handler->IsRunning()){
            std::this_thread::yield();
        }

        _frame->RunFinished();
    }


    void RunFrame::OnNavigatedTo(){
        ShowStandbyButtons();
    }
//}
