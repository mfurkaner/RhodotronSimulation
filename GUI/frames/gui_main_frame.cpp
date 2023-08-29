#include "gui_main_frame.h" 
#include "TGMsgBox.h"

namespace RhodotronSimulatorGUI::frames{


    MainFrame::MainFrame(const TGWindow*p) : TGMainFrame(p, MAIN_FRAME_W, MAIN_FRAME_H){
        SetCleanup(kDeepCleanup);
        main_buttons_frame = new MainButtonsFrame(this, MAIN_BUTTON_FRAME_W, MAIN_BUTTON_FRAME_H);
        config_frame = new ConfigurationFrame(this, CONFIG_FRAME_W, CONFIG_FRAME_H);
        render_frame = new RenderFrame(this, RENDER_FRAME_W, RENDER_FRAME_H, &renderer);
        analysis_frame = new AnalysisFrame(this, RENDER_FRAME_W, RENDER_FRAME_H, &analyzer);
        run_frame = new RunFrame(this, RENDER_FRAME_W, RENDER_FRAME_H);
        sweep_frame = new SweepFrame(this, RENDER_FRAME_W, RENDER_FRAME_H, &analyzer, &sim_handler, &dataProvider, config_frame);

        Handlers::GUIMessageBoxHandler::CreateObject(this);

        renderer.SetDataProvider(&dataProvider);
        analyzer.SetDataProvider(&dataProvider);
        run_frame->SetSimulationHandler(&sim_handler);

        this->AddFrame(main_buttons_frame, center_x_layout);
        this->AddFrame(config_frame, center_x_layout);
        this->AddFrame(run_frame, center_layout);
        this->AddFrame(render_frame, center_x_layout);
        this->AddFrame(analysis_frame, center_x_layout);
        this->AddFrame(sweep_frame, center_x_layout);

        SetName("RhodoSim_GUI");
        SetWindowName("RhodoSim GUI");

        MapSubwindows();

        Resize( GetDefaultSize() );
        MapWindow();

        this->HideFrame(render_frame);
        this->HideFrame(analysis_frame);
        this->HideFrame(run_frame);
        this->HideFrame(config_frame);
        this->HideFrame(sweep_frame);
        this->Resize(w,h);
        this->SetWMSizeHints(900, 900, 1100, 1100, 50, 50);

        NavigateToConfigFrame();
    }

    MainFrame::~MainFrame(){
        Handlers::GUIMessageBoxHandler::DestroyObject();
        DeleteWindow();
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
        NavigateToSimalateFrame();
    }

    void MainFrame::QuitPressed(){
        auto msgboxHandler = Handlers::GUIMessageBoxHandler::GetObject();
        if(msgboxHandler == NULL){
            std::cerr << "msgboxHandler is NULL!" << std::endl;
            return;
        }

        auto sure = msgboxHandler->DrawYesNoQuestion("Quitting the application", "Do you want to quit the application?");
        if(sure == MessageBoxResult::No){
            return;
        }
        
        if(sim_handler.IsRunning()){
            auto kill_sim = msgboxHandler->DrawYesNoExclamation("Simulation Running!", "A simulation is running, do you want to stop the simulation?");
            if(kill_sim == MessageBoxResult::Yes){
                sim_handler.kill_simulation();
                sim_handler.kill_server();
            }
        }
        else if(active_frame == config_frame){
            auto save_config = msgboxHandler->DrawYesNoExclamation("Configuration may not be saved!", "Do you want to save the configuration?");
            if(save_config == MessageBoxResult::Yes){
                config_frame->SaveConfigPressed();
            }
        }
        Handlers::GUIMessageBoxHandler::DestroyObject();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        DeleteWindow();
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

}