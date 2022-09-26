#include "gui_config_frame.h"


namespace RhodotronSimulatorGUI::frames{ 

    ConfigurationFrame::ConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGVerticalFrame(p, w, h){
        parent = p;
        // add the buttons
        for (int i = 0; i < configs.size(); i++){
            TGHorizontalFrame* config_line_frame = new TGHorizontalFrame( this, CONFIG_FRAME_LINE_W, CONFIG_FRAME_LINE_H);
            // Setup the button
            auto label = new TGLabel(config_line_frame, label_texts[i].c_str()); 
            auto input = new TGTextEntry( "", config_line_frame, i);
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

        for (int i = 0; i < inputs.size(); i++){
            std::string input = inputs[i]->GetText();
            if (  input.find_first_not_of(' ') == std::string::npos){
                continue;
            }
            config += configs[i] + '=';
            config += input;
            config += (i + 1 < inputs.size() ? "\n" : "");
        }

        return config;
    }


    void ConfigurationFrame::LoadConfigFromFile(const std::string& configFilePath){

        std::cout << "Inputs size: " << inputs.size() << std::endl;
        std::cout << "configs size: " << configs.size() << std::endl;
        std::ifstream configFile;   

        configFile.open(configFilePath, std::ios::in);

        std::string line;

        while ( !configFile.eof() ){

            std::getline(configFile, line);
            int i = 0;
            for (; i < configs.size(); i++){
                if (line.find("#", 0) != std::string::npos){ i = -1; break; }
                else if (line.find(configs[i], 0) != std::string::npos) {
                    break;
                }
            }
            if ( i > -1 && i < configs.size()){
                std::string cmd = line.substr( line.find('=', 0) + 1, 20);
                inputs.at(i)->SetText(cmd.c_str());
            }
        }
    }

}