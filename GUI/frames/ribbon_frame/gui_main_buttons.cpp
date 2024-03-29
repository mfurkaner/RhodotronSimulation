#include "gui_main_buttons.h"
#include "../MainFrame.h"


//namespace RhodotronSimulatorGUI::frames{


    const std::vector<std::string> MainButtonsFrame::button_names = {"Configuration", "Simulate", "Sweep", "Render", "Analyze", "Quit"};

    const std::vector<std::string> MainButtonsFrame::button_functions = {"ConfigurationPressed()", "SimulatePressed()", "SweepPressed()","RenderPressed()",
                                                                         "AnalyzePressed()",  "QuitPressed()"};

    void MainButtonsFrame::ConfigurationPressed(){
        ((MainFrame*)parent)->ConfigurationPressed();
    }
    void MainButtonsFrame::SimulatePressed(){
        ((MainFrame*)parent)->SimulatePressed();
    }
    void MainButtonsFrame::SweepPressed(){
        ((MainFrame*)parent)->SweepPressed();
    }
    void MainButtonsFrame::RenderPressed(){
        ((MainFrame*)parent)->RenderPressed();
    }
    void MainButtonsFrame::AnalyzePressed(){
        ((MainFrame*)parent)->AnalyzePressed();
    }
    void MainButtonsFrame::QuitPressed(){
        ((MainFrame*)parent)->QuitPressed();
    }

    MainButtonsFrame::MainButtonsFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGHorizontalFrame(p, w, h){
        parent = p;
        // add the buttons
        
        for (int i = 0; i < button_names.size(); i++){
            // Setup the button
            auto button = new TGTextButton(this, button_names.at(i).c_str(), -1); //= TGTextButton(this, button_names.at(i).c_str(), i );
            // add to the frame and the list
            this->AddFrame(button, center_x_layout);
            buttons.push_back(button);
        }

    }

    void MainButtonsFrame::SetupButtons(){
      for (int i = 0; i < buttons.size(); i++){
          buttons[i]->Connect("Clicked()", "MainButtonsFrame", this, button_functions.at(i).c_str());
      }
//       buttons[0]->Connect("Clicked()", "MainButtonsFrame", this, "ConfigurationPressed()");
//       buttons[5]->Connect("Clicked()", "MainButtonsFrame", this, "QuitPressed()");
//       ULong_t yellow; gClient->GetColorByName("yellow", yellow);
//       buttons[5]->ChangeBackground(yellow);
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
        for(int index = 0; index < buttons.size(); index++){
            buttons[index]->SetEnabled(true);
        }
    }

//}
