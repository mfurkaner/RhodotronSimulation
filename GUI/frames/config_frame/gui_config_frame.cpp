#include "gui_config_frame.h"

/* #include "B_config_frame/gui_B_config_frame.cpp"
#include "E_config_frame/gui_E_config_frame.cpp"
#include "gun_config_frame/gui_gun_config_frame.cpp"
#include "sim_config_frame/gui_sim_config_frame.cpp" */



namespace RhodotronSimulatorGUI::frames{ 

const std::string ConfigurationFrame::config_comment = 
           {"#                  RhodoSim Configuration File\n"
            "# ================================================================\n"
            "#    M.Furkan Er                                     22/09/2022   \n"
            "# ================================================================\n#\n"
            "# emax = Maximum electric field strength (MV/m)\n"
            "# ein = Energy of electrons coming out of the gun (MeV)\n" 
            "# einstd = Standard deviation of energy of electrons coming out of the gun (MeV)\n"
            "# targeten = Max energy on the output gif (MeV)\n"
            "# freq = Frequency of the RF field (MHz)\n"
            "# phaselag = phase lag of the first electrons (degree)\n"
            "# starttime = time to start firing the gun (ns)\n"
            "# endtime = ns to run the simulation (ns)\n"
            "# dt = time interval to do the calculations (ns)\n"
            "# guntime = how long a gun pulse is (ns)\n"
            "# gunperiod = time between two gun pulses (ns)\n"
            "# enum = number of electrons to simulate in a bunch\n"
            "# bunchnum = number of times the gun fires\n"
            "# r1 = radius of the inner cylinder (m)\n"
            "# r2 = radius of the outer cylinder (m)\n"
            "# epath = path to store the electric field data\n"
            "# bpath = path to store the magnetic field data\n"
            "# cpath = path to store the settings\n"
            "# ppath = path to store electron data\n"
            "# multh = enable or disable multitheading\n"
            "# thcount = set the maximum thread to be used\n"
            "# magrotation = degrees of rotation to enter each magnet \n"
            "# addmagnet = takes 3 input. (B , R, < Radial distance of center >)\n"
            "# output = output file name \n\n"};

    const std::vector<ConfigurationCell> ConfigurationFrame::configs = std::vector<ConfigurationCell>(
        {   ConfigurationCell(E,"emax", emax),              ConfigurationCell(Gun, "einmean", ein),
            ConfigurationCell(Gun, "einstd", einstd),
            ConfigurationCell(Gun,"targeten", targeten),   ConfigurationCell(E, "freq", frequency),
            ConfigurationCell(E,"phaselag", phaselag),      ConfigurationCell(Sim,"starttime", starttime),
            ConfigurationCell(Sim,"endtime", endtime),     ConfigurationCell(Sim,"dt", dt), 
            ConfigurationCell(Gun,"guntime", guntime),     ConfigurationCell(Gun,"gunperiod", gunperiod), 
            ConfigurationCell(Gun,"enum", _enum),          ConfigurationCell(Gun,"bunchnum", bunchnum),
            ConfigurationCell(E,"r1", _r1),                 ConfigurationCell(E,"r2", _r2), 
            ConfigurationCell(Sim,"epath", epath),         ConfigurationCell(Sim,"bpath", bpath), 
            ConfigurationCell(Sim,"cpath", cpath),         ConfigurationCell(Sim,"ppath", ppath), 
            ConfigurationCell(Sim,"multh",multh),          ConfigurationCell(Sim,"thcount",thcount),      
            ConfigurationCell(B,"magrotation", magrotation),ConfigurationCell(B,"addmagnet", addmagnet),    

        });



    ConfigurationFrame::ConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h) : TGVerticalFrame(p, w, h){
        parent = p;
        
        auto B_E_frame = new TGHorizontalFrame(this, CONFIG_FRAME_B_FRAME_W * 2, CONFIG_FRAME_B_FRAME_H);

        // Add B configuration frame
        B_config_frame = new subframes::BConfigurationFrame(B_E_frame, CONFIG_FRAME_B_FRAME_W, CONFIG_FRAME_B_FRAME_H);
        //this->AddFrame(B_config_frame, top_left_layout);

        // Add E configuration frame
        E_config_frame = new subframes::EConfigurationFrame(B_E_frame, CONFIG_FRAME_B_FRAME_W, CONFIG_FRAME_B_FRAME_H);


        // Setup B & E frame
        B_E_frame->AddFrame(B_config_frame, top_layout);
        B_E_frame->AddFrame(E_config_frame, top_layout);

        this->AddFrame(B_E_frame, center_x_layout);

        auto gun_sim_frame = new TGHorizontalFrame(this, CONFIG_FRAME_B_FRAME_W * 2, CONFIG_FRAME_B_FRAME_H);

        // Add Gun configuration frame
        gun_config_frame = new subframes::GunConfigurationFrame(gun_sim_frame, CONFIG_FRAME_B_FRAME_W, CONFIG_FRAME_B_FRAME_H);
        auto gun_frame_layout = new TGLayoutHints(kLHintsTop, 10, 50, 10, 10);
        gun_sim_frame->AddFrame(gun_config_frame, gun_frame_layout);

        // Add Sim configuration frame
        sim_config_frame = new subframes::SimConfigurationFrame(gun_sim_frame, CONFIG_FRAME_B_FRAME_W, CONFIG_FRAME_B_FRAME_H, this);
        gun_sim_frame->AddFrame(sim_config_frame, top_layout);

        this->AddFrame(gun_sim_frame, center_x_layout);
    }


    std::string ConfigurationFrame::GetConfigAsString(){
        std::string config = config_comment;

        config += "\n# E FIELD CONFIGURATION \n";
        config += E_config_frame->ProduceEConfiguration();

        config += "\n# B FIELD CONFIGURATION \n";
        config += B_config_frame->ProduceBConfiguration();

        config += "\n# GUN CONFIGURATION \n";
        config += gun_config_frame->ProduceGunConfiguration();

        config += "\n# SIM CONFIGURATION \n";
        config += sim_config_frame->ProduceSimConfiguration();

        return config;
    }


    void ConfigurationFrame::LoadConfigFromFile(const std::string& configFilePath){

        std::ifstream configFile;   

        configFile.open(configFilePath, std::ios::in);

        std::string line;
        
        int magnet_count = 1;
        std::string B_config, E_config, Gun_config, Sim_config;

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
                    case B:{
                        B_config += line + '\n';
                        break;
                    }
                    case E:{
                        E_config += line + '\n';
                        break;
                    }
                    case Gun:{
                        Gun_config += line + '\n';
                        break;
                    }
                    case Sim:{
                        Sim_config += line + '\n';
                    }
                }
            }
        }
        if ( B_config.empty() == false ){
            B_config_frame->SetBConfiguration(B_config);
        }
        if ( E_config.empty() == false ){
            E_config_frame->SetEConfiguration(E_config);
        }
        if ( Gun_config.empty() == false ){
            gun_config_frame->SetGunConfiguration(Gun_config);
        }
        if ( Sim_config.empty() == false ){
            sim_config_frame->SetSimConfiguration(Sim_config);
        }
    }

    void ConfigurationFrame::SaveConfigPressed(){
        std::string config = GetConfigAsString();
        std::ofstream config_stream, backup_stream;
        std::ifstream old_config_stream;

        old_config_stream.open(_config_file_path, std::ios::in);
        std::string old_config, line;

        while(!old_config_stream.eof()){
            std::getline(old_config_stream, line);
            old_config += line + '\n';
        }
        old_config_stream.close();

        backup_stream.open(_old_config_file_path, std::ios::out);
        backup_stream << old_config;
        backup_stream.close();

        config_stream.open(_config_file_path, std::ios::out);
        config_stream << config;
        config_stream.close();
    }

    void ConfigurationFrame::LoadConfigPressed(){
        LoadConfigFromFile(_config_file_path);
    }

}