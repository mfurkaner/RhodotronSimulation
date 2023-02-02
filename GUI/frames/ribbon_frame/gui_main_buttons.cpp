#include "gui_main_buttons.h"



namespace RhodotronSimulatorGUI::frames{


    const std::vector<std::string> MainButtonsFrame::button_names = {"Simulate", "Configuration",  "Render", "Analyze", "Quit"};

    const std::vector<std::string> MainButtonsFrame::button_functions = {"SimulatePressed()", "ConfigurationPressed()", "RenderPressed()",
                                                                         "AnalyzePressed()",  "QuitPressed()"};

    MainButtonsFrame::MainButtonsFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGHorizontalFrame(p, w, h){
        parent = p;
        // add the buttons
        for (int i = 0; i < button_names.size(); i++){
            // Setup the button
            auto button = new TGTextButton(this, button_names.at(i).c_str(), i ); //= TGTextButton(this, button_names.at(i).c_str(), i );
            button->Connect("Clicked()", "RhodotronSimulatorGUI::frames::MainFrame", (void*)parent, button_functions.at(i).c_str());
            // add to the frame and the list
            this->AddFrame(button, center_x_layout);
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
            this->Layout();
        }
    }
    
    void MainButtonsFrame::ShowByName(const std::string& name){
        int index = 0;
        for(; index < button_names.size(); index++){
            if ( button_names[index] == name ){
                break;
            }
        }
        if (index < buttons.size()){
            this->ShowFrame(buttons[index]);
            this->Layout();
        }
    }

    void MainButtonsFrame::DisableByName(const std::string& name){ 
        EnableByName(name, false);
    }

    void MainButtonsFrame::EnableByName(const std::string& name, bool enable){
        int index = 0;
        for(; index < button_names.size(); index++){
            if ( button_names[index] == name ){
                break;
            }
        }
        if (index < buttons.size()){
            buttons[index]->SetEnabled(enable);
            this->Layout();
        }
    }

    void MainButtonsFrame::EnableAll(){
        for(int index = 0; index < button_names.size(); index++){
            buttons[index]->SetEnabled(true);
        }
    }

}