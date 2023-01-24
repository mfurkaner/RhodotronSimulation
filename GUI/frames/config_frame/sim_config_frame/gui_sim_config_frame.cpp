#include "gui_sim_config_frame.h"


namespace RhodotronSimulatorGUI::frames::subframes{

    TGHorizontalFrame* SimConfigurationFrame::_init_time_entry_frame(){
        TGHorizontalFrame* time_entry_frame = new TGHorizontalFrame(this);

        auto t0_entry_frame = new TGVerticalFrame(time_entry_frame);
        auto tf_entry_frame = new TGVerticalFrame(time_entry_frame);
        auto dt_entry_frame = new TGVerticalFrame(time_entry_frame);

        // Setup Sim start time entry frame
        _t0_entry_label = new TGLabel(t0_entry_frame, sim_configuration_t0_entry_label_text.c_str());
        _t0_entry = new TGNumberEntry(t0_entry_frame, DEFAULT_SIM_T0, 7, -1, TGNumberFormat::kNESInteger,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0, 30);
        
        t0_entry_frame->AddFrame(_t0_entry_label, center_x_layout);
        t0_entry_frame->AddFrame(_t0_entry, center_x_layout);

        // Setup Sim end time entry frame
        _tf_entry_label = new TGLabel(tf_entry_frame, sim_configuration_tf_entry_label_text.c_str());
        _tf_entry = new TGNumberEntry(tf_entry_frame, DEFAULT_SIM_TF, 7, -1, TGNumberFormat::kNESInteger,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 1, 200);

        tf_entry_frame->AddFrame(_tf_entry_label, center_x_layout);
        tf_entry_frame->AddFrame(_tf_entry, center_x_layout);

        // Setup Sim time step size entry frame
        _dt_entry_label = new TGLabel(dt_entry_frame, sim_configuration_dt_entry_label_text.c_str());
        _dt_entry = new TGNumberEntry(dt_entry_frame, DEFAULT_SIM_DT, 7, -1, TGNumberFormat::kNESReal,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 0, 0.1);

        dt_entry_frame->AddFrame(_dt_entry_label, center_x_layout);
        dt_entry_frame->AddFrame(_dt_entry, center_x_layout);

        // Setup the Sim Time entry frame
        auto gun_times_frame_layout = new TGLayoutHints(kLHintsCenterY, 15, 15, 0, 0);
        time_entry_frame->AddFrame(t0_entry_frame, gun_times_frame_layout);
        time_entry_frame->AddFrame(tf_entry_frame, gun_times_frame_layout);
        time_entry_frame->AddFrame(dt_entry_frame, gun_times_frame_layout);

        return time_entry_frame;
    }

    TGVerticalFrame* SimConfigurationFrame::_init_mt_control_frame(){
        TGVerticalFrame* mt_control_frame = new TGVerticalFrame(this);

        auto thnum_entry_frame = new TGHorizontalFrame(mt_control_frame);

        // Setup Sim thread count entry frame
        _thnum_entry_label = new TGLabel(thnum_entry_frame, sim_configuration_thnum_entry_label_text.c_str());
        _thnum_entry = new TGNumberEntry(thnum_entry_frame, DEFAULT_SIM_THNUM, 7, -1, TGNumberFormat::kNESInteger,
                TGNumberFormat::kNEAPositive, TGNumberFormat::kNELLimitMinMax, 1, 10);

        thnum_entry_frame->AddFrame(_thnum_entry_label, center_y_layout);
        thnum_entry_frame->AddFrame(_thnum_entry, center_y_layout);

        // Setup Sim multithreading control checkbox
        _multh_enable_button = new TGCheckButton(mt_control_frame, sim_configuration_multh_label_text.c_str());
        _multh_enable_button->SetState(DEFAULT_SIM_MULTH_STATE);

        // Setup Sim multithreading control frame
        mt_control_frame->AddFrame(thnum_entry_frame, center_x_layout);
        mt_control_frame->AddFrame(_multh_enable_button, center_x_layout);

        return mt_control_frame;
    }

    TGHorizontalFrame* SimConfigurationFrame::_init_log_path_entry_frame(){
        TGHorizontalFrame* log_path_entry_frame = new TGHorizontalFrame(this);

        auto e_p_path_frame = new TGVerticalFrame(log_path_entry_frame);
        auto b_c_path_frame = new TGVerticalFrame(log_path_entry_frame);

        // Setup E field & particle path entry frame
        _epath_entry_label = new TGLabel(e_p_path_frame, sim_configuration_epath_entry_label_text.c_str());
        _epath_entry = new TGTextEntry(e_p_path_frame, DEFAULT_SIM_EPATH);

        _ppath_entry_label = new TGLabel(e_p_path_frame, sim_configuration_ppath_entry_label_text.c_str());
        _ppath_entry = new TGTextEntry(e_p_path_frame, DEFAULT_SIM_PPATH);

        e_p_path_frame->AddFrame(_epath_entry_label, center_x_layout);
        e_p_path_frame->AddFrame(_epath_entry, center_x_layout);
        e_p_path_frame->AddFrame(_ppath_entry_label, center_x_layout);
        e_p_path_frame->AddFrame(_ppath_entry, center_x_layout);

        // Setup B field & configuration path entry frame
        _bpath_entry_label = new TGLabel(b_c_path_frame, sim_configuration_bpath_entry_label_text.c_str());
        _bpath_entry = new TGTextEntry(b_c_path_frame, DEFAULT_SIM_BPATH);

        _cpath_entry_label = new TGLabel(b_c_path_frame, sim_configuration_cpath_entry_label_text.c_str());
        _cpath_entry = new TGTextEntry(b_c_path_frame, DEFAULT_SIM_CPATH);

        b_c_path_frame->AddFrame(_bpath_entry_label, center_x_layout);
        b_c_path_frame->AddFrame(_bpath_entry, center_x_layout);
        b_c_path_frame->AddFrame(_cpath_entry_label, center_x_layout);
        b_c_path_frame->AddFrame(_cpath_entry, center_x_layout);

        // Setup Log Paths entry frame
        log_path_entry_frame->AddFrame(e_p_path_frame, center_y_layout);
        log_path_entry_frame->AddFrame(b_c_path_frame, center_y_layout);
        
        return log_path_entry_frame;
    }

    SimConfigurationFrame::SimConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGVerticalFrame(p, w, h){
        parent = p;

        _frame_title = new TGLabel(this, sim_configuration_title.c_str());
        _frame_title->SetTextFont(FONT_BOLD_14);

        auto time_entry_frame = _init_time_entry_frame();
        auto mt_control_frame = _init_mt_control_frame();
        auto log_path_entry_frame = _init_log_path_entry_frame();

        auto title_layout = new TGLayoutHints(kLHintsCenterX, 5, 5, 0, 10);
        this->AddFrame(_frame_title,title_layout);

        this->AddFrame(time_entry_frame, center_x_layout);
        this->AddFrame(mt_control_frame, center_x_layout);
        this->AddFrame(log_path_entry_frame, center_x_layout);
    }


    std::string SimConfigurationFrame::ProduceSimConfiguration(){
        std::string simConfiguration;

        for (  const auto cell : ConfigurationFrame::configs ){
            if ( cell.Type != Sim )
                continue;
            
            std::string line = cell.Value + '=';

            char temp[50];

            switch (cell.Index)
            {
                case starttime:{
                    snprintf(temp, 50, "%d", (int)_t0_entry->GetNumber());
                    line += temp;
                    break;
                }
                case endtime:{
                    snprintf(temp, 50, "%d", (int)_tf_entry->GetNumber());
                    line += temp;
                    break;
                }
                case dt:{
                    snprintf(temp, 50, "%lf",_dt_entry->GetNumber());
                    line += temp;
                    break;
                }
                case thcount:{
                    snprintf(temp, 50, "%d",(int)_thnum_entry->GetNumber());
                    // Disabled
                    line = '#' + line + temp;
                    break;
                }
                case multh:{
                    snprintf(temp, 50, "%d",(int)_multh_enable_button->IsDown());
                    // Disabled
                    line = '#' + line + temp;
                    break;
                }
                case epath:{
                    std::string epath_ = _epath_entry->GetText();
                    line += epath_;
                    break;
                }
                case bpath:{
                    std::string bpath_ = _bpath_entry->GetText();
                    line += bpath_;
                    break;
                }
                case ppath:{
                    std::string ppath_ = _ppath_entry->GetText();
                    line += ppath_;
                    break;
                }
                case cpath:{
                    std::string cpath_ = _cpath_entry->GetText();
                    line += cpath_;
                    break;
                }
                default:
                    std::cerr << "Unknown command in ProduceSimConfiguration: " << cell.Value << std::endl;
                    break;
            }

            simConfiguration += line + '\n';
        }

        return simConfiguration;
    }


        void SimConfigurationFrame::SetSimConfiguration(std::string config){
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
                if ( cell.Type == Sim && cell.Value == cmd){
                    command = cell.Index;
                    break;
                }
            }

            switch (command)
            {
                case starttime:{
                    _t0_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case endtime:{
                    _tf_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case dt:{
                    _dt_entry->SetNumber(atof(value.c_str()));
                    break;
                }
                case thcount:{
                    _thnum_entry->SetNumber(atoi(value.c_str()));
                    break;
                }
                case multh:{
                    // TODO : Add multh functionality here
                    break;
                }
                case epath:{
                    _epath_entry->SetText(value.c_str());
                    break;
                }
                case bpath:{
                    _bpath_entry->SetText(value.c_str());
                    break;
                }
                case ppath:{
                    _ppath_entry->SetText(value.c_str());
                    break;
                }
                case cpath:{
                    _cpath_entry->SetText(value.c_str());
                    break;
                }
                default:
                    std::cerr << "Unknown command in SetGunConfiguration: " << line << std::endl;
                    break;
            }
        }
    }
}