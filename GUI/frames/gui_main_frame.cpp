#include "gui_main_frame.h" 
#include "gui_config_frame.h"

namespace RhodotronSimulatorGUI::frames{


    MainFrame::MainFrame(const TGWindow*p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h){
        SetCleanup(kDeepCleanup);
        main_buttons_frame = std::make_unique<MainButtonsFrame>(this, MAIN_BUTTON_FRAME_W, MAIN_BUTTON_FRAME_H);
        config_frame = std::make_unique<ConfigurationFrame>(this, CONFIG_FRAME_W, CONFIG_FRAME_H);

        this->AddFrame(main_buttons_frame.get(), center_layout);
        this->AddFrame(config_frame.get(), center_layout);

        SetName("RhodoSim_GUI");
        SetWindowName("RhodoSim GUI");

        MapSubwindows();

        Resize( GetDefaultSize() );
        MapWindow();
    }

    MainFrame::~MainFrame(){
        DeleteWindow();
    }

    void MainFrame::SaveConfigPressed(){
        std::string config = config_frame.get()->GetConfigAsString();
        std::cout << config << std::endl;

        std::ofstream config_stream;

        config_stream.open(_config_file_path, std::ios::out);

        config_stream << config;

        config_stream.close();
    }

    void MainFrame::LoadConfigPressed(){
    }

    void MainFrame::SaveGifPressed() {
    }

    void MainFrame::RunPressed(){

    }

    void MainFrame::StopPressed(){

    }

    void MainFrame::QuitPressed(){
    }




}