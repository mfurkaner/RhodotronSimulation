#include "gui_config_frame.h"


namespace RhodotronSimulatorGUI::frames{ 

    ConfigurationFrame::ConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGVerticalFrame(p, w, h){
        parent = p;
        
        auto B_E_frame = new TGHorizontalFrame(this, CONFIG_FRAME_B_FRAME_W * 2, CONFIG_FRAME_B_FRAME_H);

        // Add B configuration frame
        B_config_frame = new subframes::BConfigurationFrame(B_E_frame, CONFIG_FRAME_B_FRAME_W, CONFIG_FRAME_B_FRAME_H);
        //this->AddFrame(B_config_frame, top_left_layout);

        // Add E configuration frame
        E_config_frame = new subframes::EConfigurationFrame(B_E_frame, CONFIG_FRAME_B_FRAME_W, CONFIG_FRAME_B_FRAME_H);


        // Setup B & E frame
        B_E_frame->AddFrame(B_config_frame, center_y_layout);
        B_E_frame->AddFrame(E_config_frame, center_y_layout);

        this->AddFrame(B_E_frame, center_x_layout);

        auto gun_sim_frame = new TGHorizontalFrame(this, CONFIG_FRAME_B_FRAME_W * 2, CONFIG_FRAME_B_FRAME_H);

        // Add Gun configuration frame
        gun_config_frame = new subframes::GunConfigurationFrame(gun_sim_frame, CONFIG_FRAME_B_FRAME_W, CONFIG_FRAME_B_FRAME_H);
        auto gun_frame_layout = new TGLayoutHints(kLHintsLeft, 10, 400, 10, 10);
        gun_sim_frame->AddFrame(gun_config_frame, center_y_layout);

        // Add Sim configuration frame
        sim_config_frame = new subframes::SimConfigurationFrame(gun_sim_frame, CONFIG_FRAME_B_FRAME_W, CONFIG_FRAME_B_FRAME_H);
        gun_sim_frame->AddFrame(sim_config_frame, center_y_layout);

        this->AddFrame(gun_sim_frame, center_x_layout);
    }


    std::string ConfigurationFrame::GetConfigAsString(){
        std::string config = config_comment;

        config += "\n# E FIELD CONFIGURATION \n";
        config += E_config_frame->ProduceEConfiguration();

        config += "\n# B FIELD CONFIGURATION \n";
        config += B_config_frame->ProduceBConfiguration();

        config += "\n# GUN CONFIGURATION \n";
        config += gun_config_frame->ProduceGunConfiguration();

        config += "\n# SIM CONFIGURATION \n";
        config += sim_config_frame->ProduceSimConfiguration();

        return config;
    }


    void ConfigurationFrame::LoadConfigFromFile(const std::string& configFilePath){

        std::cout << "configs size: " << configs.size() << std::endl;
        std::ifstream configFile;   

        configFile.open(configFilePath, std::ios::in);

        std::string line;
        
        int magnet_count = 1;
        std::string B_config, E_config, Gun_config, Sim_config;

        while ( !configFile.eof() ){

            std::getline(configFile, line);
            int i = 0;
            for (; i < configs.size(); i++){
                if (line.find("#", 0) != std::string::npos){ i = -1; break; }
                else if (line.find(configs[i].Value, 0) != std::string::npos) {
                    break;
                }
            }
            if ( i > -1 && i < configs.size()){
                std::string cmd = line.substr( line.find('=', 0) + 1, 50);
                switch (configs[i].Type)
                {
                    case B:{
                        B_config += line + '\n';
                        break;
                    }
                    case E:{
                        E_config += line + '\n';
                        break;
                    }
                    case Gun:{
                        Gun_config += line + '\n';
                        break;
                    }
                    case Sim:{
                        Sim_config += line + '\n';
                    }
                }
            }
        }
        if ( B_config.empty() == false ){
            B_config_frame->SetBConfiguration(B_config);
        }
        if ( E_config.empty() == false ){
            E_config_frame->SetEConfiguration(E_config);
        }
        if ( Gun_config.empty() == false ){
            gun_config_frame->SetGunConfiguration(Gun_config);
        }
        if ( Sim_config.empty() == false ){
            sim_config_frame->SetSimConfiguration(Sim_config);
        }
    }

}