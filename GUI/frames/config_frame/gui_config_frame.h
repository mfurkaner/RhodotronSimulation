#ifndef GUI_CONFIG_FRAME_H
#define GUI_CONFIG_FRAME_H



#include "TSystem.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGTextView.h"
#include "TGLabel.h"
#include "TGTextEntry.h"
#include "TGListBox.h"
#include "TGLViewer.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TImage.h"
#include "TASImage.h"

#include <stdio.h>
#include <string.h>
#include <vector>
#include <tuple>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <spawn.h>
#include <sched.h>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <fstream>

#include "../gui_frames_common.h"
#include "B_config_frame/gui_B_config_frame.h"
#include "E_config_frame/gui_E_config_frame.h"
#include "gun_config_frame/gui_gun_config_frame.h"
#include "sim_config_frame/gui_sim_config_frame.h"

namespace RhodotronSimulatorGUI::frames{

// TODO : DO THIS DYNAMICALLY
    enum AvailableConfigIndex{
        emax, frequency, phaselag,
        epath, bpath, ppath, cpath,
        starttime, endtime, dt,
        guntime, _enum, multh,
        thcount, _r1, _r2, ein,
        addmagnet, magrotation,
        bunchnum, gunperiod,
        targeten, output,
        debug,

        endofconfig
    };

    enum ConfigType{
        B, E, Gun, Sim
    };
    struct ConfigurationCell{
        ConfigType Type;
        std::string Value;
        AvailableConfigIndex Index;

        ConfigurationCell(ConfigType type, std::string value, AvailableConfigIndex index) : Type(type), Value(value), Index(index){}
    };

    class ConfigurationFrame : public TGVerticalFrame{
        subframes::BConfigurationFrame* B_config_frame;
        subframes::EConfigurationFrame* E_config_frame;
        subframes::GunConfigurationFrame* gun_config_frame;
        subframes::SimConfigurationFrame* sim_config_frame;

        

        const TGWindow* parent;
    public:
        
        static const std::vector<ConfigurationCell> configs;


        static const std::string config_comment;
        
        ConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h);


        std::string GetConfigAsString();

        void LoadConfigFromFile(const std::string& configFilePath);

        std::vector<TGFrame*>& GetInputs();

        int GetEnum(){ return (gun_config_frame ? gun_config_frame->GetEnum() : 0); };
        int GetBnum(){ return (gun_config_frame ? gun_config_frame->GetBnum() : 0); };
        // TODO : Get this from sim config frame
        float GetStartTime(){return (sim_config_frame ? sim_config_frame->GetStartTime() : 0); }
        float GetEndTime(){return (sim_config_frame ? sim_config_frame->GetEndTime() : 0);}

        float GetR1(){return (E_config_frame ? E_config_frame->GetR1() : 0);}
        float GetR2(){return (E_config_frame ? E_config_frame->GetR2() : 0);}

        float GetTargetEnergy(){return (gun_config_frame ? gun_config_frame->GetTargetEn() : 0); }

        const std::string GetPPath(){return (sim_config_frame ? sim_config_frame->GetPPath() : "xy/paths/");}
        const std::string GetEPath(){return (sim_config_frame ? sim_config_frame->GetEPath() : "xy/rf.txt");}
        const std::string GetBPath(){return (sim_config_frame ? sim_config_frame->GetBPath() : "xy/magnet.txt");}
        const std::string GetCPath(){return (sim_config_frame ? sim_config_frame->GetCPath() : "xy/settings.txt");}
    };


}


#endif