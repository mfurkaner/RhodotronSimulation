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

        std::ofstream config_stream;

        config_stream.open(_config_file_path, std::ios::out);

        config_stream << config;

        config_stream.close();
    }

    void MainFrame::LoadConfigPressed(){
        config_frame->LoadConfigFromFile(_config_file_path);
    }

    void MainFrame::SaveGifPressed() {
    }

    void MainFrame::RunPressed(){
        std::cout << "Simulation started" << std::endl;

        SaveConfigPressed();

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