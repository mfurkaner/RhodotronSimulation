#include "gui_config_frame.h"


namespace RhodotronSimulatorGUI::frames{ 

    ConfigurationFrame::ConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGVerticalFrame(p, w, h){
        parent = p;
        // add the buttons
        for (int i = 0; i < endofconfig; i++){
            TGHorizontalFrame* config_line_frame = new TGHorizontalFrame( this, CONFIG_FRAME_LINE_W, CONFIG_FRAME_LINE_H);
            // Setup the button
            auto label = new TGLabel(config_line_frame, label_texts[i].c_str()); 
            TGFrame* input;
            switch (configs[i].Type)
            {
                case Text:
                    input = new TGTextEntry( "", config_line_frame, i);
                    input->Resize(100, 25);
                    break;
                case TextList:
                    input = new TGListBox(config_line_frame, i);
                    input->Resize(200, 75);
                    break;
                default:
                    break;
            }

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

        for (int i = 0; i < endofconfig; i++){
            std::string input;
            switch (configs[i].Type)
            {
            case Text:
                input = ((TGTextEntry*)inputs.at(i))->GetText();
                if (  input.find_first_not_of(' ') == std::string::npos){
                    continue;
                }
                config += configs[i].Value + '=';
                config += input;
                break;
            case TextList:
            
                auto listbox = ((TGListBox*)inputs.at(i));
                std::cout << "There are " << listbox->GetNumberOfEntries() << " entries" << std::endl;

                listbox->ls();
                
                for( int j = 1; j <= listbox->GetNumberOfEntries(); j++ ){
                    std::cout << "Reading " << j << " th entry" << std::endl;

                    auto entry = (TGTextLBEntry*)listbox->GetEntry(j);
                    entry->ls();
                    auto text_in_the_list = entry->GetText();
                    
                    config += configs[i].Value + '=';
                    config += text_in_the_list->GetString();

                    config += (j + 1 <= listbox->GetNumberOfEntries() ? "\n" : "");
                }
                break;
            }

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
        
        int magnet_count = 1;

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
                case Text:
                    ((TGTextEntry*)inputs.at(i))->SetText(cmd.c_str());
                    break;
                case TextList:
                    auto listbox = ((TGListBox*)inputs.at(i));
                    listbox->AddEntry(cmd.c_str(), magnet_count);
                    listbox->Layout();
                    magnet_count++;
                    break;
                }
                
            }
        }
    }

    std::vector<TGFrame*>& ConfigurationFrame::GetInputs(){
        return inputs;
    }

}