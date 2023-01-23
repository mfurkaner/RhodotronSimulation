#include "gui_E_config_frame.h"
#include "../gui_config_frame.h"
#include <sstream>

namespace RhodotronSimulatorGUI::frames::subframes{

    TGHorizontalFrame* EConfigurationFrame::_init_cavity_desc_frame(){
        TGHorizontalFrame* cavity_desc_frame = new TGHorizontalFrame(this);

        TGVerticalFrame* input_side_frame = new TGVerticalFrame(cavity_desc_frame);

        TGHorizontalFrame* r1_frame = new TGHorizontalFrame(input_side_frame);
        TGHorizontalFrame* r2_frame = new TGHorizontalFrame(input_side_frame);

        _r1_entry_label = new TGLabel(r1_frame, E_configuration_R1_entry_label_text.c_str());
        _r1_entry = new TGNumberEntry(r1_frame, 0, 5, -1, TGNumberFormat::kNESRealThree,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMax, 0, 3);

        r1_frame->AddFrame(_r1_entry_label, center_layout);
        r1_frame->AddFrame(_r1_entry, center_layout);

        _r2_entry_label = new TGLabel(r2_frame, E_configuration_R2_entry_label_text.c_str());
        _r2_entry = new TGNumberEntry(r2_frame, 0.1, 5, -1, TGNumberFormat::kNESRealThree,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMax, 0.01, 3);

        r2_frame->AddFrame(_r2_entry_label, center_layout);
        r2_frame->AddFrame(_r2_entry, center_layout);

        auto input_r1_layout = new TGLayoutHints(kLHintsRight, 50, 30, 75, 15);
        auto input_r2_layout = new TGLayoutHints(kLHintsRight, 50, 30, 15, 75);

        input_side_frame->AddFrame(r1_frame, input_r1_layout);
        input_side_frame->AddFrame(r2_frame, input_r2_layout);

        auto pic = (TGPicture*)gClient->GetPicture(E_configuration_cavity_desc_pic_path.c_str(), E_CONFIGURATION_CAVITY_DESC_PIC_SIZE, E_CONFIGURATION_CAVITY_DESC_PIC_SIZE);
        _cavity_description_canvas = new TGIcon(cavity_desc_frame, pic, E_CONFIGURATION_CAVITY_DESC_PIC_SIZE, E_CONFIGURATION_CAVITY_DESC_PIC_SIZE);

        cavity_desc_frame->AddFrame(input_side_frame, center_layout);
        cavity_desc_frame->AddFrame(_cavity_description_canvas, center_layout);

        return cavity_desc_frame;
    }

    TGHorizontalFrame* EConfigurationFrame::_init_rf_desc_frame(){
        TGHorizontalFrame* rf_desc_frame = new TGHorizontalFrame(this);

        TGVerticalFrame* input_side_frame = new TGVerticalFrame(rf_desc_frame);

        TGHorizontalFrame* Emax_frame = new TGHorizontalFrame(input_side_frame);
        TGHorizontalFrame* freq_frame = new TGHorizontalFrame(input_side_frame);
        TGHorizontalFrame* phase_lag_frame = new TGHorizontalFrame(input_side_frame);

        _Emax_entry_label = new TGLabel(Emax_frame, E_configuration_Emax_entry_label_text.c_str());
        _Emax_entry = new TGNumberEntry(Emax_frame, 0, 5, -1, TGNumberFormat::kNESRealThree,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMax, 0, 10);

        Emax_frame->AddFrame(_Emax_entry_label, center_layout);
        Emax_frame->AddFrame(_Emax_entry, center_layout);

        _freq_entry_label = new TGLabel(freq_frame, E_configuration_freq_entry_label_text.c_str());
        _freq_entry = new TGNumberEntry(freq_frame, 0.1, 5, -1, TGNumberFormat::kNESRealOne,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMax, 0.0, 500);

        freq_frame->AddFrame(_freq_entry_label, center_layout);
        freq_frame->AddFrame(_freq_entry, center_layout);

        _phase_lag_entry_label = new TGLabel(phase_lag_frame, E_configuration_phase_lag_entry_label_text.c_str());
        _phase_lag_entry = new TGNumberEntry(phase_lag_frame, 0.1, 5, -1, TGNumberFormat::kNESRealOne,
                TGNumberFormat::kNEAAnyNumber, TGNumberFormat::kNELLimitMax, -180, 180);

        phase_lag_frame->AddFrame(_phase_lag_entry_label, center_layout);
        phase_lag_frame->AddFrame(_phase_lag_entry, center_layout);

        auto input_Emax_layout = new TGLayoutHints(kLHintsRight, 10, 10, 80, 15);
        auto input_freq_layout = new TGLayoutHints(kLHintsRight, 10, 10, 15, 15);
        auto input_phase_lag_layout = new TGLayoutHints(kLHintsRight, 10, 10, 15, 80);

        input_side_frame->AddFrame(Emax_frame, input_Emax_layout);
        input_side_frame->AddFrame(freq_frame, input_freq_layout);
        input_side_frame->AddFrame(phase_lag_frame, input_phase_lag_layout);

        auto pic = (TGPicture*)gClient->GetPicture(E_configuration_rf_desc_pic_path.c_str(), E_CONFIGURATION_RF_DESC_PIC_SIZE, E_CONFIGURATION_RF_DESC_PIC_SIZE);
        _rf_description_canvas = new TGIcon(rf_desc_frame, pic, E_CONFIGURATION_RF_DESC_PIC_SIZE, E_CONFIGURATION_RF_DESC_PIC_SIZE);

        rf_desc_frame->AddFrame(input_side_frame, center_layout);
        rf_desc_frame->AddFrame(_rf_description_canvas, center_layout);

        return rf_desc_frame;
    }


    EConfigurationFrame::EConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGVerticalFrame(p, w, h){
        parent = p;
        
        auto cavity_frame = _init_cavity_desc_frame();
        auto rf_frame = _init_rf_desc_frame();

        this->AddFrame(cavity_frame, center_layout);
        this->AddFrame(rf_frame, center_layout);
    }


    std::string EConfigurationFrame::ProduceEConfiguration(){
        std::string eConfiguration;

        for (  const auto cell : ConfigurationFrame::configs ){
            if ( cell.Type != E )
                continue;
            
            eConfiguration += cell.Value + '=';

            char temp[50];

            switch (cell.Index)
            {
                case _r1:{
                    snprintf(temp, 50, "%.4lf",_r1_entry->GetNumber());
                    eConfiguration += temp;
                    break;
                }
                case _r2:{
                    snprintf(temp, 50, "%.4lf",_r2_entry->GetNumber());
                    eConfiguration += temp;
                    break;
                }
                case emax:{
                    snprintf(temp, 50, "%.3lf",_Emax_entry->GetNumber());
                    eConfiguration += temp;
                    break;
                }
                case frequency:{
                    snprintf(temp, 50, "%.1lf",_freq_entry->GetNumber());
                    eConfiguration += temp;
                    break;
                }
                case phaselag:{
                    snprintf(temp, 50, "%.1lf",_phase_lag_entry->GetNumber());
                    eConfiguration += temp;
                    break;
                }
                default:
                    std::cerr << "Unknown command in ProduceEConfiguration: " << cell.Value << std::endl;
                    break;
            }

            eConfiguration += '\n';
        }

        return eConfiguration;
    }   

    void EConfigurationFrame::SetEConfiguration(std::string config){
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

            for( const auto cell : ConfigurationFrame::configs ){
                if ( cell.Type == E && cell.Value == cmd){
                    command = cell.Index;
                    break;
                }
            }

            switch (command)
            {
                case _r1:{
                    _r1_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case _r2:{
                    _r2_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case emax:{
                    _Emax_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case frequency:{
                    _freq_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case phaselag:{
                    _phase_lag_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                default:
                    std::cerr << "Unknown command in SetEConfiguration: " << line << std::endl;
                    break;
            }
        }
    }
    
}