#include "gui_main_buttons.h"



namespace RhodotronSimulatorGUI::frames{

    MainButtonsFrame::MainButtonsFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGHorizontalFrame(p, w, h){
        parent = p;
        // add the buttons
        for (int i = 0; i < button_names.size(); i++){
            // Setup the button
            auto button = std::make_shared<TGTextButton>(this, button_names.at(i).c_str(), i ); //= TGTextButton(this, button_names.at(i).c_str(), i );
            button->Connect("Clicked()", "RhodotronSimulatorGUI::frames::MainFrame", (void*)parent, button_functions.at(i).c_str());
            // add to the frame and the list
            this->AddFrame(button.get(), center_layout);
            buttons.push_back(button);
        }
    }

}