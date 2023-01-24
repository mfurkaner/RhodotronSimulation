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

        static const std::string config_comment;

        const TGWindow* parent;
    public:
        static const std::vector<ConfigurationCell> configs;
        ConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h);


        std::string GetConfigAsString();

        void LoadConfigFromFile(const std::string& configFilePath);

        std::vector<TGFrame*>& GetInputs();

        int GetEnum(){ return (gun_config_frame ? gun_config_frame->GetEnum() : 0); };
        int GetBnum(){ return (gun_config_frame ? gun_config_frame->GetBnum() : 0); };
        // TODO : Get this from sim config frame
        double GetStartTime(){return (sim_config_frame ? sim_config_frame->GetStartTime() : 0); }
        double GetEndTime(){return (sim_config_frame ? sim_config_frame->GetEndTime() : 0);}

        double GetTargetEnergy(){return (gun_config_frame ? gun_config_frame->GetTargetEn() : 0); }
    };

    const std::vector<ConfigurationCell> ConfigurationFrame::configs = 
       {ConfigurationCell(E,"emax", emax),              ConfigurationCell(Gun, "ein", ein),
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
};


    const std::string ConfigurationFrame::config_comment = 
               {"#                  RhodoSim Configuration File\n"
                "# ================================================================\n"
                "#    M.Furkan Er                                     22/09/2022   \n"
                "# ================================================================\n#\n"
                "# emax = Maximum electric field strength (MV/m)\n"
                "# ein = Energy of electrons coming out of the gun (MeV)\n" 
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
}

#endif