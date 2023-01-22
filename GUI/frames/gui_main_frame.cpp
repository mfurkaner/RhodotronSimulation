#include "gui_main_frame.h" 

namespace RhodotronSimulatorGUI::frames{


    MainFrame::MainFrame(const TGWindow*p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h){
        SetCleanup(kDeepCleanup);
        main_buttons_frame = new MainButtonsFrame(this, MAIN_BUTTON_FRAME_W, MAIN_BUTTON_FRAME_H);
        config_frame = new ConfigurationFrame(this, CONFIG_FRAME_W, CONFIG_FRAME_H);
        render_frame = new RenderFrame(this, RENDER_FRAME_W, RENDER_FRAME_H);

        sim_handler.set_progress_bar(render_frame->GetProgressBar());

        this->AddFrame(main_buttons_frame, center_layout);
        this->AddFrame(config_frame, center_layout);
        this->AddFrame(render_frame, center_layout);

        SetName("RhodoSim_GUI");
        SetWindowName("RhodoSim GUI");

        MapSubwindows();

        Resize( GetDefaultSize() );
        MapWindow();

        this->HideFrame(render_frame);
    }

    MainFrame::~MainFrame(){
        DeleteWindow();
    }

    void MainFrame::SaveConfigPressed(){
        std::string config = config_frame->GetConfigAsString();
        std::cout << config << std::endl;

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

    void MainFrame::RenderPressed() {
        this->HideFrame(config_frame);
        this->ShowFrame(render_frame);
        auto inputs = config_frame->GetInputs();

        LoadConfigPressed();
        
        // TODO : This is stupid and unsafe!!!!
        int _enum = atoi(((TGTextEntry*)inputs[10])->GetText());
        int _bnum = atoi(((TGTextEntry*)inputs[11])->GetText());

        std::cout << "enum: " << _enum <<  " bnum: " << _bnum << std::endl;

        render_frame->SetEnum(_enum * _bnum);

        render_frame->Render();
        
        main_buttons_frame->HideByName("Render");
    }

    void MainFrame::RunPressed(){
        std::cout << "Simulation started" << std::endl;

        //SaveConfigPressed();

        sim_handler.spawn_server();
        sim_handler.spawn_simulation();

        this->HideFrame(config_frame);
        this->ShowFrame(render_frame);
    }

    void MainFrame::StopPressed(){
        sim_handler.kill_simulation();
        sim_handler.kill_server();
    }

    void MainFrame::QuitPressed(){
        sim_handler.join_server();
        DeleteWindow();
    }




}