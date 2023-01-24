#include "gui_B_config_frame.h"
#include <sstream>

namespace RhodotronSimulatorGUI::frames::subframes{

#pragma region INIT_SUB_FRAMES
    TGHorizontalFrame* BConfigurationFrame::_init_magnet_edit_frame(){
        auto magnet_edit_frame = new TGHorizontalFrame(this, CONFIG_FRAME_LINE_W, CONFIG_FRAME_LINE_H);

        _B_entry_label = new TGLabel(magnet_edit_frame, B_configuration_B_entry_label_text.c_str());
        _B_entry = new TGNumberEntry(magnet_edit_frame, 0.0001,5, -1, TGNumberFormat::kNESReal, 
                TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMinMax, -5, 5);
        _B_entry->Resize(75,20);
                                
        _R_entry_label = new TGLabel(magnet_edit_frame, B_configuration_R_entry_label_text.c_str());
        _R_entry = new TGNumberEntry(magnet_edit_frame, 0.0001,3, -1, TGNumberFormat::kNESRealTwo, 
                TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 2);
        _R_entry->Resize(75,20);
        
        _r_entry_label = new TGLabel(magnet_edit_frame, B_configuration_r_entry_label_text.c_str());
        _r_entry = new TGNumberEntry(magnet_edit_frame, 0.0001,4, -1, TGNumberFormat::kNESRealThree, 
                TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 3);
        _r_entry->Resize(75,20);

        magnet_edit_frame->AddFrame(_B_entry_label, center_y_layout);
        magnet_edit_frame->AddFrame(_B_entry, center_x_layout);
        magnet_edit_frame->AddFrame(_R_entry_label, center_y_layout);
        magnet_edit_frame->AddFrame(_R_entry, center_x_layout);
        magnet_edit_frame->AddFrame(_r_entry_label, center_y_layout);
        magnet_edit_frame->AddFrame(_r_entry, center_x_layout);

        return magnet_edit_frame;
    }

    TGHorizontalFrame* BConfigurationFrame::_init_magnet_edit_buttons_frame(){
        TGHorizontalFrame* magnet_edit_buttons_frame = new TGHorizontalFrame(this, CONFIG_FRAME_LINE_W, CONFIG_FRAME_LINE_H);

        _add_magnet_button = new TGTextButton(magnet_edit_buttons_frame, "Add");
        _add_magnet_button->Connect("Clicked()", "RhodotronSimulatorGUI::frames::subframes::BConfigurationFrame",
            this, "AddPressed()");

        _save_magnet_button = new TGTextButton(magnet_edit_buttons_frame, "Save");
        _save_magnet_button->Connect("Clicked()", "RhodotronSimulatorGUI::frames::subframes::BConfigurationFrame",
            this, "SavePressed()");

        _delete_magnet_button = new TGTextButton(magnet_edit_buttons_frame, "Delete");
        _delete_magnet_button->Connect("Clicked()", "RhodotronSimulatorGUI::frames::subframes::BConfigurationFrame",
            this, "DeletePressed()");

        magnet_edit_buttons_frame->AddFrame(_add_magnet_button, center_x_layout);
        magnet_edit_buttons_frame->AddFrame(_save_magnet_button, center_x_layout);
        magnet_edit_buttons_frame->AddFrame(_delete_magnet_button, center_x_layout);

        return magnet_edit_buttons_frame;
    }

    TGHorizontalFrame* BConfigurationFrame::_init_mag_rotation_edit_frame(){
        TGHorizontalFrame* mag_rotation_edit_frame = new TGHorizontalFrame(this, CONFIG_FRAME_LINE_W, CONFIG_FRAME_LINE_H);

        _mag_rotation_entry_label = new TGLabel(mag_rotation_edit_frame, B_configuration_mag_rot_entry_label_text.c_str());
        _mag_rotation_entry = new TGNumberEntry(mag_rotation_edit_frame, DEFAULT_B_MAG_ROT,0, -1, TGNumberFormat::kNESRealOne, 
                TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 30);
        _mag_rotation_entry->Resize(50,20);


        mag_rotation_edit_frame->AddFrame(_mag_rotation_entry_label, center_y_layout);
        mag_rotation_edit_frame->AddFrame(_mag_rotation_entry, center_x_layout);
        return mag_rotation_edit_frame;
    }
#pragma endregion INIT_SUB_FRAMES

    BConfigurationFrame::BConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGVerticalFrame(p, w, h){
        parent = p;

        _frame_title = new TGLabel(this, B_configuration_title.c_str());
        _frame_title->SetTextFont(FONT_BOLD_14);

        _magnets_listbox = new TGListBox(this, -1);
        _magnets_listbox->Resize(275, 250);
        _magnets_listbox->GetContainer()->Connect("DoubleClicked(TGFrame*, Int_t)", 
        "RhodotronSimulatorGUI::frames::subframes::BConfigurationFrame",
        this, "MagnetDoubleClicked(TGFrame*, Int_t)");

        LoadDefaultMagnets();

        TGHorizontalFrame* magnet_edit_frame = _init_magnet_edit_frame();
        TGHorizontalFrame* magnet_edit_buttons_frame = _init_magnet_edit_buttons_frame();
        TGHorizontalFrame* mag_rotation_edit_frame = _init_mag_rotation_edit_frame();

        auto mag_line_frames_layout = new TGLayoutHints(kLHintsCenterX, 10, 10, 15, 15);
        auto mag_last_line_frame_layout = new TGLayoutHints(kLHintsCenterX, 10, 10, 15, 5);
        auto title_layout = new TGLayoutHints(kLHintsCenterX | kLHintsTop, 5, 5, 5, 20);
        this->AddFrame(_frame_title, title_layout);
        this->AddFrame(_magnets_listbox, center_x_layout);
        this->AddFrame(magnet_edit_frame, mag_line_frames_layout);
        this->AddFrame(magnet_edit_buttons_frame, mag_line_frames_layout);
        this->AddFrame(mag_rotation_edit_frame, mag_last_line_frame_layout);
    }

    void BConfigurationFrame::AddPressed(){
        MagnetConfigurationDetails new_magnet;
        new_magnet.B = _B_entry->GetNumber();
        new_magnet.R = _R_entry->GetNumber();
        new_magnet.r = _r_entry->GetNumber();

        AddMagnet(new_magnet);
    }

    void BConfigurationFrame::SavePressed(){
        int index = _magnets_listbox->GetSelected();
        if ( index < 1 || index >= _magnets.size() )
            return;
        
        _magnets[index - 1].B = _B_entry->GetNumber();
        _magnets[index - 1].R = _R_entry->GetNumber();
        _magnets[index - 1].r = _r_entry->GetNumber();

        UpdateMagnetList();
    }

    void BConfigurationFrame::DeletePressed(){
        int index = _magnets_listbox->GetSelected();
        if ( index < 1)
            return;
        // TODO : SAFETY CHECK FOR ID - INDEX CORRELATION

        _magnets.erase(_magnets.begin() + index - 1);

        UpdateMagnetList();
    }

    void BConfigurationFrame::MagnetDoubleClicked(TGFrame *f, Int_t btn){
        if (btn != 1)
            return;

        int index = _magnets_listbox->GetSelected();
        if ( index < 1 || index > _magnets.size() )
            return;

        // TODO : SAFETY CHECK FOR ID - INDEX CORRELATION
        _B_entry->SetNumber(_magnets[index - 1].B);
        _R_entry->SetNumber(_magnets[index - 1].R);
        _r_entry->SetNumber(_magnets[index - 1].r);
    }

    void BConfigurationFrame::UpdateMagnetList(){
        _magnets_listbox->RemoveAll();

        for ( int i = 0; i < _magnets.size(); i++){
            std::string new_magnet_desc = std::to_string(i + 1) + ") ";
            new_magnet_desc += _magnets[i].GetDescription();

            _magnets_listbox->NewEntry(new_magnet_desc.c_str());
        }
    }

    std::string BConfigurationFrame::ProduceBConfiguration(){
        std::string bConfiguration;
        char temp[100];
        snprintf(temp, 100, "magrotation=%.1lf\n", _mag_rotation_entry->GetNumber());
        bConfiguration = temp;

        for(int i = 0; i < _magnets.size(); i++){
            bConfiguration += "addmagnet=" + _magnets[i].GetConfiguration() + '\n';
        }

        return bConfiguration;
    }

    void BConfigurationFrame::SetBConfiguration(std::string config){
        // TODO : Check input
        std::stringstream config_stream(config);
        std::string line;
        _magnets.clear();
        UpdateMagnetList();

        while( config_stream.eof() == false ){
            std::getline(config_stream, line);
            if( line.find('#') != std::string::npos){
                continue;
            }
            std::string cmd = line.substr(0, line.find('='));
            std::string value = line.substr( line.find('=', 0) + 1, 50);
            if(cmd.empty() || value.empty()){
                continue;
            }
            
            if(cmd == "magrotation"){
                _mag_rotation_entry->SetNumber(atof(value.c_str()));
            }

            else if (cmd == "addmagnet"){
                AddMagnet(value);
            }
            else{
                std::cerr << "Unknown command " << cmd << std::endl;
            }
        }
    }

    void BConfigurationFrame::AddMagnet(std::string config){
        // TODO : Check input
        auto new_magnet = MagnetConfigurationDetails(config);
        _magnets.push_back(new_magnet);
        UpdateMagnetList();
    }

    void BConfigurationFrame::AddMagnet(MagnetConfigurationDetails new_magnet){
        _magnets.push_back(new_magnet);
        UpdateMagnetList();
    }

    void BConfigurationFrame::LoadDefaultMagnets(){
        _magnets.clear();
        for(int i = 0; i < DEFAULT_B_MAG_COUNT; i++){
            std::string mag = std::to_string(DEFAULT_B_MAG_B[i]) + " , "
            + std::to_string(DEFAULT_B_MAG_R[i]) + " , <" + std::to_string(DEFAULT_B_MAG_r[i]) + ">";
            AddMagnet(mag);
        }
        UpdateMagnetList();
    }

    MagnetConfigurationDetails::MagnetConfigurationDetails(std::string config){
        std::string first, second, third;
        size_t first_comma, second_comma;

        first_comma = config.find_first_of(',');
        second_comma = first_comma + 1;
        second_comma += config.substr(first_comma + 1).find_first_of(',');

        first = config.substr(0, first_comma);
        second = config.substr(first_comma + 1, second_comma);
        third = config.substr(second_comma + 1);

        third = third.substr(third.find_first_of('<') + 1, third.find_first_of('>'));

        B = atof(first.c_str());
        R = atof(second.c_str());
        r = atof(third.c_str());
    }
}