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

        B_E_frame->AddFrame(B_config_frame, center_layout);
        B_E_frame->AddFrame(E_config_frame, center_layout);

        this->AddFrame(B_E_frame, center_layout);

        // add the buttons
        
        for (int i = 0; i < configs.size(); i++){
            
            if(configs[i].Type != Text){
                continue;
            }

            TGHorizontalFrame* config_line_frame = new TGHorizontalFrame( this, CONFIG_FRAME_LINE_W, CONFIG_FRAME_LINE_H);
            // Setup the button
            auto label = new TGLabel(config_line_frame, label_texts[i].c_str()); 
            TGFrame* input;

            input = new TGTextEntry( "", config_line_frame, i);
            input->Resize(100, 25);
            // add to the frame and the list
            config_line_frame->AddFrame(label, bottom_layout);
            config_line_frame->AddFrame(input, right_layout);

            labels.push_back(label);
            inputs.push_back(input);

            this->AddFrame(config_line_frame, right_layout);
        }
    }


    std::string ConfigurationFrame::GetConfigAsString(){
        std::string config = config_comment;

        for (int i = 0, j=0; i < configs.size(); i++){
            if (configs[i].Type != Text){
                continue;
            }

            std::cout << "Exporting config text type " << i << std::endl;
            std::string input;

            input = ((TGTextEntry*)inputs.at(j))->GetText();    
            j++;
            if (  input.find_first_not_of(' ') == std::string::npos){
                continue;
            }
            config += configs[i].Value + '=';
            config += input + '\n';
        }

        std::cout << "End of text type export " << std::endl;

        config += "# E FIELD CONFIGURATION \n";
        config += E_config_frame->ProduceEConfiguration();

        config += "# B FIELD CONFIGURATION \n";
        config += B_config_frame->ProduceBConfiguration();

        return config;
    }


    void ConfigurationFrame::LoadConfigFromFile(const std::string& configFilePath){

        std::cout << "Inputs size: " << inputs.size() << std::endl;
        std::cout << "configs size: " << configs.size() << std::endl;
        std::ifstream configFile;   

        configFile.open(configFilePath, std::ios::in);

        std::string line;
        
        int magnet_count = 1;
        std::string B_config, E_config;

        while ( !configFile.eof() ){

            std::getline(configFile, line);
            int i = 0, j=0;
            for (; i < configs.size(); i++){
                if (line.find("#", 0) != std::string::npos){ i = -1; break; }
                else if (line.find(configs[i].Value, 0) != std::string::npos) {
                    break;
                }
                else if (configs[i].Type == Text){
                    j++;
                }
            }
            if ( i > -1 && i < configs.size()){
                std::string cmd = line.substr( line.find('=', 0) + 1, 50);
                switch (configs[i].Type)
                {
                    case Text:{
                        ((TGTextEntry*)inputs.at(j))->SetText(cmd.c_str());
                        break;
                    }
                    case TextList:{
                        /*
                        auto listbox = ((TGListBox*)inputs.at(i));
                        listbox->AddEntry(cmd.c_str(), magnet_count);
                        listbox->Layout();
                        magnet_count++;*/
                        break;
                    }
                    case B:{
                        B_config += line + '\n';
                        break;
                    }
                    case E:{
                        E_config += line + '\n';
                        break;
                    }
                }
            }
        }
        if ( B_config.empty() == false ){
            std::cout << "Getting B config " << std::endl << B_config << std::endl;
            B_config_frame->SetBConfiguration(B_config);
        }
        if ( E_config.empty() == false ){
            std::cout << "Getting E config " << std::endl << E_config << std::endl;
            E_config_frame->SetEConfiguration(E_config);
        }
    }

    std::vector<TGFrame*>& ConfigurationFrame::GetInputs(){
        return inputs;
    }

}