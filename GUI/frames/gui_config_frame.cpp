#include "gui_config_frame.h"


namespace RhodotronSimulatorGUI::frames{ 

    ConfigurationFrame::ConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGVerticalFrame(p, w, h){
        parent = p;
        // add the buttons
        for (int i = 0; i < configs.size(); i++){
            TGHorizontalFrame* config_line_frame = new TGHorizontalFrame( this, CONFIG_FRAME_LINE_W, CONFIG_FRAME_LINE_H);
            // Setup the button
            auto label = std::make_shared<TGLabel>(config_line_frame, label_texts[i].c_str()); 
            auto input = std::make_shared<TGTextEntry>( "", config_line_frame, i);
            input.get()->Resize(100, 25);
            // add to the frame and the list
            config_line_frame->AddFrame(label.get(), bottom_layout);
            config_line_frame->AddFrame(input.get(), right_layout);

            labels.push_back(label);
            inputs.push_back(input);

            this->AddFrame(config_line_frame, right_layout);
        }
    }


    std::string ConfigurationFrame::GetConfigAsString(){
        std::string config = config_comment;

        for (int i = 0; i < inputs.size(); i++){
            config += configs[i] + " = ";
            config += inputs[i].get()->GetText();
            config += (i + 1 < inputs.size() ? "\n" : "");
        }

        return config;
    }



}