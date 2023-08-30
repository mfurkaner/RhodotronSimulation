#include "gui_gun_config_frame.h"
#include "../../../data/gui_data_types.h"

#include <sstream>

//namespace RhodotronSimulatorGUI::frames::subframes{

    TGHorizontalFrame* GunConfigurationFrame::_init_Ein_icon_frame(){
        auto Ein_icon_frame = new TGHorizontalFrame(this);

        TGVerticalFrame* Ein_entry_frame = new TGVerticalFrame(Ein_icon_frame);
        TGHorizontalFrame* Ein_Einstd_frame = new TGHorizontalFrame(Ein_entry_frame);

        // Setup Gun Ein entry frame
        _Ein_entry_label = new TGLabel(Ein_entry_frame, gun_configuration_Ein_entry_label_text.c_str());

        _Ein_entry = new TGNumberEntry(Ein_Einstd_frame, DEFAULT_GUN_EIN, 5, -1, TGNumberFormat::kNESRealThree,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0.001, 5);
        _EinStd_entry_label = new TGLabel(Ein_Einstd_frame, "+-");
        _EinStd_entry = new TGNumberEntry(Ein_Einstd_frame, 0, 5, -1, TGNumberFormat::kNESRealThree,
                TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 0.01);

        Ein_Einstd_frame->AddFrame(_Ein_entry);
        Ein_Einstd_frame->AddFrame(_EinStd_entry_label);
        Ein_Einstd_frame->AddFrame(_EinStd_entry);

        _targetE_entry_label = new TGLabel(Ein_entry_frame, gun_configuration_targetE_entry_label_text.c_str());
        _targetE_entry = new TGNumberEntry(Ein_entry_frame, DEFAULT_GUN_TARGETE, 5, -1, TGNumberFormat::kNESRealOne,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0.1, 5);
        
        Ein_entry_frame->AddFrame(_Ein_entry_label, center_x_layout);
        Ein_entry_frame->AddFrame(Ein_Einstd_frame, center_x_layout);
        Ein_entry_frame->AddFrame(_targetE_entry_label, center_x_layout);
        Ein_entry_frame->AddFrame(_targetE_entry, center_x_layout);

        // Setup Gun Desctiontion icon
        auto pic = (TGPicture*)gClient->GetPicture(gun_configuration_gun_desc_pic_path.c_str(),
             GUN_CONFIGURATION_GUN_DESC_PIC_SIZE_X, GUN_CONFIGURATION_GUN_DESC_PIC_SIZE_Y);
        _gun_description_canvas = new TGIcon(Ein_icon_frame, pic, 
        GUN_CONFIGURATION_GUN_DESC_PIC_SIZE_X, GUN_CONFIGURATION_GUN_DESC_PIC_SIZE_Y);

        // Setup Ein&icon frame
        auto Ein_entry_layout = new TGLayoutHints(kLHintsCenterY, 10, 10, 15, 5);
        Ein_icon_frame->AddFrame(Ein_entry_frame, Ein_entry_layout);
    
        auto icon_layout = new TGLayoutHints(kLHintsCenterY, 10, 10, 10, 0);
        Ein_icon_frame->AddFrame(_gun_description_canvas, icon_layout);

        return Ein_icon_frame;
    }

    TGHorizontalFrame* GunConfigurationFrame::_init_gun_times_frame(){
        TGHorizontalFrame* gun_times_frame = new TGHorizontalFrame(this);

        auto T_entry_frame = new TGVerticalFrame(gun_times_frame);
        auto tg_entry_frame = new TGVerticalFrame(gun_times_frame);

        // Setup Gun T entry frame
        _T_entry_label = new TGLabel(T_entry_frame, gun_configuration_T_entry_label_text.c_str());
        _T_entry = new TGNumberEntry(T_entry_frame, DEFAULT_GUN_T, 5, -1, TGNumberFormat::kNESRealOne,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0, 30);
        
        T_entry_frame->AddFrame(_T_entry_label, center_x_layout);
        T_entry_frame->AddFrame(_T_entry, center_x_layout);

        // Setup Gun tg entry frame
        _tg_entry_label = new TGLabel(tg_entry_frame, gun_configuration_tg_entry_label_text.c_str());
        _tg_entry = new TGNumberEntry(tg_entry_frame, DEFAULT_GUN_TG, 5, -1, TGNumberFormat::kNESRealOne,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0.1, 10);

        tg_entry_frame->AddFrame(_tg_entry_label, center_x_layout);
        tg_entry_frame->AddFrame(_tg_entry, center_x_layout);

        // Setup the Gun Times entry frame
        auto gun_times_frame_layout = new TGLayoutHints(kLHintsCenterY, 15, 15, 0, 0);
        gun_times_frame->AddFrame(T_entry_frame, gun_times_frame_layout);
        gun_times_frame->AddFrame(tg_entry_frame, gun_times_frame_layout);

        return gun_times_frame;
    }

    TGHorizontalFrame* GunConfigurationFrame::_init_particle_samples_frame(){
        TGHorizontalFrame* particle_samples_frame = new TGHorizontalFrame(this);

        auto bnum_entry_frame = new TGVerticalFrame(particle_samples_frame);
        auto enum_entry_frame = new TGVerticalFrame(particle_samples_frame);

        // Setup Gun bnum entry frame
        _bnum_entry_label = new TGLabel(bnum_entry_frame, gun_configuration_bnum_entry_label_text.c_str());
        _bnum_entry = new TGNumberEntry(bnum_entry_frame, DEFAULT_GUN_BNUM, 5, -1, TGNumberFormat::kNESInteger,
                TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 30);
        
        bnum_entry_frame->AddFrame(_bnum_entry_label, center_x_layout);
        bnum_entry_frame->AddFrame(_bnum_entry, center_x_layout);

        // Setup Gun enum entry frame
        _enum_entry_label = new TGLabel(enum_entry_frame, gun_configuration_enum_entry_label_text.c_str());
        _enum_entry = new TGNumberEntry(enum_entry_frame, DEFAULT_GUN_ENUM, 5, -1, TGNumberFormat::kNESInteger,
                TGNumberFormat::kNEANonNegative, TGNumberFormat::kNELLimitMinMax, 0, 300);

        enum_entry_frame->AddFrame(_enum_entry_label, center_x_layout);
        enum_entry_frame->AddFrame(_enum_entry, center_x_layout);

        // Setup the Gun particles entry frame
        auto particle_samples_frame_layout = new TGLayoutHints(kLHintsCenterY, 38, 38, 0, 5);
        particle_samples_frame->AddFrame(bnum_entry_frame, particle_samples_frame_layout);
        particle_samples_frame->AddFrame(enum_entry_frame, particle_samples_frame_layout);

        return particle_samples_frame;
    }


    GunConfigurationFrame::GunConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGVerticalFrame(p, w, h){
        parent = p;

        _frame_title = new TGLabel(this, gun_configuration_title.c_str());
        _frame_title->SetTextFont(FONT_BOLD_14);

        auto Ein_icon_frame = _init_Ein_icon_frame();
        auto gun_times_frame = _init_gun_times_frame();
        auto particle_samples_frame = _init_particle_samples_frame();

        auto title_layout = new TGLayoutHints(kLHintsCenterX, 5, 5, 0, 0);
        this->AddFrame(_frame_title, title_layout);

        auto Ein_icon_layout = new TGLayoutHints(kLHintsCenterX, 5, 5, 10, 0);
        this->AddFrame(Ein_icon_frame, Ein_icon_layout);

        auto gun_times_layout = new TGLayoutHints(kLHintsCenterX, 5, 5, 0, 0);
        this->AddFrame(gun_times_frame, gun_times_layout);
        auto particle_samples_layout = new TGLayoutHints(kLHintsCenterX, 5, 5, 0, 5);
        this->AddFrame(particle_samples_frame, particle_samples_layout);
    }


    std::string GunConfigurationFrame::ProduceGunConfiguration(){
        std::string gunConfiguration;

        for (  const auto cell : configs ){
            if ( cell.Type != Gun )
                continue;
            
            gunConfiguration += cell.Value + '=';

            char temp[50];

            switch (cell.Index)
            {
                case ein:{
                    snprintf(temp, 50, "%.3f",_Ein_entry->GetNumber());
                    gunConfiguration += temp;
                    break;
                }
                case einstd:{
                    snprintf(temp, 50, "%.4f",_EinStd_entry->GetNumber());
                    gunConfiguration += temp;
                    break;
                }
                case guntime:{
                    snprintf(temp, 50, "%.1f",_tg_entry->GetNumber());
                    gunConfiguration += temp;
                    break;
                }
                case gunperiod:{
                    snprintf(temp, 50, "%.1f",_T_entry->GetNumber());
                    gunConfiguration += temp;
                    break;
                }
                case bunchnum:{
                    snprintf(temp, 50, "%d",(int)_bnum_entry->GetNumber());
                    gunConfiguration += temp;
                    break;
                }
                case _enum:{
                    snprintf(temp, 50, "%d",(int)_enum_entry->GetNumber());
                    gunConfiguration += temp;
                    break;
                }
                case targeten:{
                    snprintf(temp, 50, "%.1f",_targetE_entry->GetNumber());
                    gunConfiguration += temp;
                    break;
                }
                default:
                    #ifdef DEBUG
                    std::cerr << "Unknown command in ProduceGunConfiguration: " << cell.Value << std::endl;
                    #endif
                    break;
            }

            gunConfiguration += '\n';
        }

        return gunConfiguration;
    }


    void GunConfigurationFrame::SetGunConfiguration(std::string config){
       // TODO : Check input
        std::stringstream config_stream(config);

        std::string line;

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

            AvailableConfigIndex command = endofconfig;

            for( const auto cell : configs ){
                if ( cell.Type == Gun && cell.Value == cmd){
                    command = cell.Index;
                    break;
                }
            }

            switch (command)
            {
                case ein:{
                    _Ein_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case einstd:{
                    _EinStd_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case guntime:{
                    _tg_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case gunperiod:{
                    _T_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case bunchnum:{
                    _bnum_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case _enum:{
                    _enum_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case targeten:{
                    _targetE_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                default:
                    #ifdef DEBUG
                    std::cerr << "Unknown command in SetGunConfiguration: " << line << std::endl;
                    #endif
                    break;
            }
        }
    }
//}