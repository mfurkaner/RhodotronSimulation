#include "MainFrame.h" 
#include "TGMsgBox.h"

//namespace RhodotronSimulatorGUI::frames{

ClassImp(MainFrame)


    MainFrame::MainFrame(const TGWindow*p) : TGMainFrame(p, MAIN_FRAME_W, MAIN_FRAME_H){
        SetCleanup(kDeepCleanup);
        SetName("RhodoSim_GUI");
        SetWindowName("RhodoSim GUI");
        this->Resize(w,h);
        this->SetWMSizeHints(900, 900, 1100, 1100, 50, 50);
        
        main_buttons_frame = new MainButtonsFrame(this, MAIN_BUTTON_FRAME_W, MAIN_BUTTON_FRAME_H);
        config_frame = new ConfigurationFrame(this, CONFIG_FRAME_W, CONFIG_FRAME_H);
        render_frame = new RenderFrame(this, RENDER_FRAME_W, RENDER_FRAME_H, &renderer);
        analysis_frame = new AnalysisFrame(this, RENDER_FRAME_W, RENDER_FRAME_H, &analyzer);
        run_frame = new RunFrame(this, RENDER_FRAME_W, RENDER_FRAME_H);
        sweep_frame = new SweepFrame(this, RENDER_FRAME_W, RENDER_FRAME_H, &analyzer, &sim_handler, &dataProvider, config_frame);

        GUIMessageBoxHandler::CreateObject(this);

        renderer.SetDataProvider(&dataProvider);
        analyzer.SetDataProvider(&dataProvider);
        run_frame->SetSimulationHandler(&sim_handler);
        run_frame->SetDataProvider(&dataProvider);
        run_frame->SetRenderer(&renderer);

        this->AddFrame(main_buttons_frame, center_x_layout);
        this->AddFrame(config_frame, center_x_layout);
        this->AddFrame(run_frame, center_layout);
        this->AddFrame(render_frame, center_x_layout);
        this->AddFrame(analysis_frame, center_x_layout);
        this->AddFrame(sweep_frame, center_x_layout);

        MapSubwindows();
        MapWindow();

        this->HideFrame(render_frame);
        this->HideFrame(analysis_frame);
        this->HideFrame(run_frame);
        this->HideFrame(config_frame);
        this->HideFrame(sweep_frame);
        Print();

        NavigateToConfigFrame();
    }

    MainFrame::~MainFrame(){
        GUIMessageBoxHandler::DestroyObject();
        DeleteWindow();
    }

    void MainFrame::SetupFrame(){
        main_buttons_frame->SetupButtons();

        analysis_frame->Setup();
        config_frame->Setup();
        render_frame->Setup();
        run_frame->Setup();
        sweep_frame->Setup();
    }

    void MainFrame::ClearData(){
        renderer.clear();
        dataProvider.clearLogs();
    }

    void MainFrame::ConfigurationPressed(){
        NavigateToConfigFrame();
    }

    void MainFrame::RenderPressed() {
        NavigateToRenderFrame();
        // TODO : remove these by add-ing configuration to mainframe and giving the address to render frame

        config_frame->SaveConfigPressed();
        config_frame->LoadCompletedConfig();

        if ( dataProvider.isDataFilled() == false){
            std::cout << "Filling dataProvider" << std::endl;
            dataProvider.clearLogs();
            dataProvider.SetEnum(config_frame->GetEnum());
            dataProvider.SetBnum(config_frame->GetBnum());
            dataProvider.SetElogPath(config_frame->GetPPath());
            dataProvider.SetRflogPath(config_frame->GetEPath());
            dataProvider.SetStaticBfieldlogPath(config_frame->GetBPath());
            dataProvider.fillLogs();
        }

        render_frame->SetupRenderer();
        render_frame->SetTargetEnergy(config_frame->GetTargetEnergy());
        render_frame->SetTimeInterval(config_frame->GetStartTime(), config_frame->GetEndTime());
        render_frame->SetR1R2(config_frame->GetR1(), config_frame->GetR2());
        renderer.Render();
    }

    void MainFrame::AnalyzePressed(){
        NavigateToAnalysisFrame();

        config_frame->SaveConfigPressed();
        config_frame->LoadCompletedConfig();

        if ( dataProvider.isDataFilled() == false){
            dataProvider.clearLogs();
            dataProvider.SetEnum(config_frame->GetEnum());
            dataProvider.SetBnum(config_frame->GetBnum());
            dataProvider.SetElogPath(config_frame->GetPPath());
            dataProvider.SetRflogPath(config_frame->GetEPath());
            dataProvider.SetStaticBfieldlogPath(config_frame->GetBPath());
            dataProvider.fillLogs();
        }
        analysis_frame->SetTimeInterval(config_frame->GetStartTime(), config_frame->GetEndTime());
        analysis_frame->SetEnumBnum(config_frame->GetEnum(), config_frame->GetBnum());
        analysis_frame->SetupAnalyzer();
    }

    void MainFrame::SweepPressed(){
        config_frame->SaveConfigPressed();

        dataProvider.clearLogs();
        dataProvider.SetEnum(config_frame->GetEnum());
        dataProvider.SetBnum(config_frame->GetBnum());
        dataProvider.SetElogPath(config_frame->GetPPath());
        dataProvider.SetRflogPath(config_frame->GetEPath());
        dataProvider.SetStaticBfieldlogPath(config_frame->GetBPath());

        NavigateToSweepFrame();
    }

    void MainFrame::SimulatePressed(){
        config_frame->SaveConfigPressed();
        dataProvider.clearLogs();
        dataProvider.SetEnum(config_frame->GetEnum());
        dataProvider.SetBnum(config_frame->GetBnum());
        dataProvider.SetElogPath(config_frame->GetPPath());
        dataProvider.SetRflogPath(config_frame->GetEPath());
        dataProvider.SetStaticBfieldlogPath(config_frame->GetBPath());
        NavigateToSimalateFrame();
    }

    void MainFrame::QuitPressed(){
        auto msgboxHandler = GUIMessageBoxHandler::GetObject();
        if(msgboxHandler == NULL){
            #ifdef DEBUG
            std::cerr << "msgboxHandler is NULL!" << std::endl;
            #endif
            return;
        }

        auto sure = msgboxHandler->DrawYesNoQuestion(
            Run_frame_mbox_title_quitting.c_str(), Run_frame_mbox_msg_quitting.c_str());
        if(sure == MessageBoxResult::No){
            return;
        }
        
        if(sim_handler.IsRunning()){
            auto kill_sim = msgboxHandler->DrawYesNoExclamation(
                Run_frame_mbox_title_quitting_while_simulation.c_str(), Run_frame_mbox_msg_quitting_while_simulation.c_str());
            if(kill_sim == MessageBoxResult::Yes){
                sim_handler.kill_simulation();
                sim_handler.kill_server();
            }
            else{
                return;
            }
        }
        else if(active_frame == config_frame){
            auto save_config = msgboxHandler->DrawYesNoExclamation(
                Run_frame_mbox_title_quitting_save_conf.c_str(), Run_frame_mbox_msg_quitting_save_conf.c_str());
            if(save_config == MessageBoxResult::Yes){
                config_frame->SaveConfigPressed();
            }
        }
        GUIMessageBoxHandler::DestroyObject();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        DeleteWindow();
        exit(EXIT_SUCCESS);
    }

    void MainFrame::NavigateTo(TGFrame* childFrame){
        if(childFrame->GetParent() != this || active_frame == childFrame){
            return;
        }

        this->HideFrame(active_frame);
        this->ShowFrame(childFrame);
        active_frame = childFrame;
    }


    void MainFrame::NavigateToConfigFrame(){
        this->Resize(w, h);

        config_frame->LoadConfigPressed();

        main_buttons_frame->EnableAll();
        main_buttons_frame->DisableByName("Configuration");
        NavigateTo(config_frame);
    }

    void MainFrame::NavigateToSimalateFrame(){
        this->Resize(SIM_FRAME_W, SIM_FRAME_H + MAIN_BUTTON_FRAME_H + common_padding);
        main_buttons_frame->EnableAll();
        main_buttons_frame->DisableByName("Simulate");
        run_frame->OnNavigatedTo();
        NavigateTo(run_frame);
    }

    void MainFrame::NavigateToAnalysisFrame(){
        this->Resize(ANALYSIS_FRAME_W, ANALYSIS_FRAME_H + MAIN_BUTTON_FRAME_H + common_padding);
        main_buttons_frame->EnableAll();
        main_buttons_frame->DisableByName("Analyze");
        NavigateTo(analysis_frame);
        analysis_frame->OnNavigatedTo();
    }

    void MainFrame::NavigateToSweepFrame(){
        this->Resize(ANALYSIS_FRAME_W, ANALYSIS_FRAME_H + MAIN_BUTTON_FRAME_H + common_padding);
        main_buttons_frame->EnableAll();
        main_buttons_frame->DisableByName("Sweep");
        NavigateTo(sweep_frame);
        sweep_frame->OnNavigatedTo();
    }

    void MainFrame::NavigateToRenderFrame(){
        this->Resize(RENDER_FRAME_W, RENDER_FRAME_H + MAIN_BUTTON_FRAME_H + common_padding);
        main_buttons_frame->EnableAll();
        main_buttons_frame->DisableByName("Render");
        NavigateTo(render_frame);
        render_frame->OnNavigatedTo();
    }

//}
