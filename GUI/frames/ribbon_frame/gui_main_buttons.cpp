#include "gui_main_buttons.h"



namespace RhodotronSimulatorGUI::frames{

    MainButtonsFrame::MainButtonsFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGHorizontalFrame(p, w, h){
        parent = p;
        // add the buttons
        for (int i = 0; i < button_names.size(); i++){
            // Setup the button
            auto button = new TGTextButton(this, button_names.at(i).c_str(), i ); //= TGTextButton(this, button_names.at(i).c_str(), i );
            button->Connect("Clicked()", "RhodotronSimulatorGUI::frames::MainFrame", (void*)parent, button_functions.at(i).c_str());
            // add to the frame and the list
            this->AddFrame(button, center_layout);
            buttons.push_back(button);
        }
    }

    void MainButtonsFrame::HideByName(const std::string& name){
        int index = 0;
        for(; index < button_names.size(); index++){
            if ( button_names[index] == name ){
                break;
            }
        }
        if (index < buttons.size()){
            this->HideFrame(buttons[index]);
        }
    }

}