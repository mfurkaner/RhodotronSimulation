#include "gui_main_frame.h" 

namespace RhodotronSimulatorGUI::frames{


    MainFrame::MainFrame(const TGWindow*p) : TGMainFrame(p, MAIN_FRAME_W, MAIN_FRAME_H){
        SetCleanup(kDeepCleanup);
        main_buttons_frame = new MainButtonsFrame(this, MAIN_BUTTON_FRAME_W, MAIN_BUTTON_FRAME_H);
        config_frame = new ConfigurationFrame(this, CONFIG_FRAME_W, CONFIG_FRAME_H);
        render_frame = new RenderFrame(this, RENDER_FRAME_W, RENDER_FRAME_H, &renderer);
        analysis_frame = new AnalysisFrame(this, RENDER_FRAME_W, RENDER_FRAME_H, &analyzer);
        run_frame = new RunFrame(this, RENDER_FRAME_W, RENDER_FRAME_H);

        renderer.SetDataProvider(&dataProvider);
        analyzer.SetDataProvider(&dataProvider);

        run_frame->SetSimulationHandler(&sim_handler);

        this->AddFrame(main_buttons_frame, center_x_layout);
        this->AddFrame(config_frame, center_x_layout);
        this->AddFrame(run_frame, center_layout);
        this->AddFrame(render_frame, center_x_layout);
        this->AddFrame(analysis_frame, center_x_layout);

        SetName("RhodoSim_GUI");
        SetWindowName("RhodoSim GUI");

        MapSubwindows();

        Resize( GetDefaultSize() );
        MapWindow();

        this->HideFrame(render_frame);
        this->HideFrame(analysis_frame);
        this->HideFrame(run_frame);
        this->HideFrame(config_frame);
        this->Resize(w,h);
        this->SetWMSizeHints(900, 900, 1100, 1100, 50, 50);

        NavigateToConfigFrame();
    }

    MainFrame::~MainFrame(){
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

        float _starttime = config_frame->GetStartTime();
        float _endtime = config_frame->GetEndTime();
        float _targetEn = config_frame->GetTargetEnergy();
        float _r1 = config_frame->GetR1();
        float _r2 = config_frame->GetR2();

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
        render_frame->SetTargetEnergy(_targetEn);
        render_frame->SetTimeInterval(_starttime, _endtime);
        render_frame->SetR1R2(_r1, _r2);
        renderer.Render();
    }

    void MainFrame::AnalyzePressed(){
        NavigateToAnalysisFrame();
        float _starttime = config_frame->GetStartTime();
        float _endtime = config_frame->GetEndTime();
        int _enum = config_frame->GetEnum();
        int _bnum = config_frame->GetBnum();
        if ( dataProvider.isDataFilled() == false){
            dataProvider.clearLogs();
            dataProvider.SetEnum(config_frame->GetEnum());
            dataProvider.SetBnum(config_frame->GetBnum());
            dataProvider.SetElogPath(config_frame->GetPPath());
            dataProvider.SetRflogPath(config_frame->GetEPath());
            dataProvider.SetStaticBfieldlogPath(config_frame->GetBPath());
            dataProvider.fillLogs();
        }
        analysis_frame->SetTimeInterval(_starttime, _endtime);
        analysis_frame->SetEnumBnum(_enum, _bnum);
        analysis_frame->SetupAnalyzer();
    }

    void MainFrame::SimulatePressed(){
        config_frame->SaveConfigPressed();
        NavigateToSimalateFrame();
    }

    void MainFrame::QuitPressed(){
        sim_handler.join_server();
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
        main_buttons_frame->EnableAll();
        main_buttons_frame->DisableByName("Configuration");
        NavigateTo(config_frame);
    }

    void MainFrame::NavigateToSimalateFrame(){
        this->Resize(SIM_FRAME_W, SIM_FRAME_H + MAIN_BUTTON_FRAME_H + common_padding);
        main_buttons_frame->EnableAll();
        main_buttons_frame->DisableByName("Simalate");
        NavigateTo(run_frame);
    }

    void MainFrame::NavigateToAnalysisFrame(){
        this->Resize(ANALYSIS_FRAME_W, ANALYSIS_FRAME_H + MAIN_BUTTON_FRAME_H + common_padding);
        main_buttons_frame->EnableAll();
        main_buttons_frame->DisableByName("Analyze");
        NavigateTo(analysis_frame);
        analysis_frame->OnNavigatedTo();
    }

    void MainFrame::NavigateToRenderFrame(){
        this->Resize(RENDER_FRAME_W, RENDER_FRAME_H + MAIN_BUTTON_FRAME_H + common_padding);
        main_buttons_frame->EnableAll();
        main_buttons_frame->DisableByName("Render");
        NavigateTo(render_frame);
    }

}