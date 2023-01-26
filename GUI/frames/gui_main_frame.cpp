#include "gui_main_frame.h" 

namespace RhodotronSimulatorGUI::frames{


    MainFrame::MainFrame(const TGWindow*p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h){
        SetCleanup(kDeepCleanup);
        main_buttons_frame = new MainButtonsFrame(this, MAIN_BUTTON_FRAME_W, MAIN_BUTTON_FRAME_H);
        config_frame = new ConfigurationFrame(this, CONFIG_FRAME_W, CONFIG_FRAME_H);
        render_frame = new RenderFrame(this, RENDER_FRAME_W, RENDER_FRAME_H);

        sim_handler.set_progress_bar(render_frame->GetProgressBar());

        this->AddFrame(main_buttons_frame, center_x_layout);
        this->AddFrame(config_frame, center_x_layout);
        this->AddFrame(render_frame, center_x_layout);

        SetName("RhodoSim_GUI");
        SetWindowName("RhodoSim GUI");

        MapSubwindows();

        Resize( GetDefaultSize() );
        MapWindow();

        this->HideFrame(render_frame);
        active_frame = config_frame;
    }

    MainFrame::~MainFrame(){
        DeleteWindow();
    }

    void MainFrame::SaveConfigPressed(){
        std::string config = config_frame->GetConfigAsString();
        
        std::ofstream config_stream, backup_stream;
        std::ifstream old_config_stream;

        old_config_stream.open(_config_file_path, std::ios::in);
        std::string old_config, line;

        while(!old_config_stream.eof()){
            std::getline(old_config_stream, line);
            old_config += line + '\n';
        }
        old_config_stream.close();

        backup_stream.open(_old_config_file_path, std::ios::out);
        backup_stream << old_config;
        backup_stream.close();

        config_stream.open(_config_file_path, std::ios::out);
        config_stream << config;
        config_stream.close();
    }

    void MainFrame::LoadConfigPressed(){
        config_frame->LoadConfigFromFile(_config_file_path);
    }

    void MainFrame::ConfigurationPressed(){
        NavigateTo(config_frame, "Configuration");
        main_buttons_frame->ShowByName("Save Config");
        main_buttons_frame->ShowByName("Load Config");
    }

    void MainFrame::RenderPressed() {
        NavigateTo(render_frame, "Render");
        
        int _enum = config_frame->GetEnum();
        int _bnum = config_frame->GetBnum();

        // TODO : Get this from sim config frame
        float _starttime = config_frame->GetStartTime();
        float _endtime = config_frame->GetEndTime();

        float _targetEn = config_frame->GetTargetEnergy();

        std::cout << "enum: " << _enum <<  " bnum: " << _bnum << std::endl;

        render_frame->SetEnum(_enum);
        render_frame->SetBnum(_bnum);
        render_frame->SetTargetEnergy(_targetEn);
        render_frame->SetTimeInterval(_starttime, _endtime);

        render_frame->Render();
    }

    void MainFrame::RunPressed(){
        std::cout << "Simulation started" << std::endl;

        SaveConfigPressed();

        sim_handler.spawn_server();
        sim_handler.spawn_simulation();

        NavigateTo(render_frame, "Run");
        main_buttons_frame->ShowByName("Stop");
        main_buttons_frame->ShowByName("Configuration");
        main_buttons_frame->HideByName("Save Config");
        main_buttons_frame->HideByName("Load Config");
    }

    void MainFrame::StopPressed(){
        sim_handler.kill_simulation();
        sim_handler.kill_server();
    }

    void MainFrame::QuitPressed(){
        sim_handler.join_server();
        DeleteWindow();
    }


    void MainFrame::NavigateTo(TGFrame* childFrame, std::string hideButton = ""){
        if(childFrame->GetParent() != this || active_frame == childFrame){
            return;
        }

        this->HideFrame(active_frame);
        this->ShowFrame(childFrame);
        active_frame = childFrame;

        if ( hideButton.empty() == false ){
            main_buttons_frame->HideByName(hideButton);
        }
    }


}