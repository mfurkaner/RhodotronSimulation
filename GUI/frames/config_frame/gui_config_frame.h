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
        Text, TextList, B, E
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

        static const std::string config_comment;
        static const std::vector<std::string> label_texts;
        std::vector<TGLabel*> labels;
        std::vector<TGFrame*> inputs;

        const TGWindow* parent;
    public:
        static const std::vector<ConfigurationCell> configs;
        ConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h);


        std::string GetConfigAsString();

        void LoadConfigFromFile(const std::string& configFilePath);

        std::vector<TGFrame*>& GetInputs();

    };

    const std::vector<ConfigurationCell> ConfigurationFrame::configs = 
       {ConfigurationCell(E,"emax", emax),              ConfigurationCell(Text, "ein", ein),
        ConfigurationCell(Text,"targeten", targeten),   ConfigurationCell(E, "freq", frequency),
        ConfigurationCell(E,"phaselag", phaselag),      ConfigurationCell(Text,"starttime", starttime),
        ConfigurationCell(Text,"endtime", endtime),     ConfigurationCell(Text,"dt", dt), 
        ConfigurationCell(Text,"guntime", guntime),     ConfigurationCell(Text,"gunperiod", gunperiod), 
        ConfigurationCell(Text,"enum", _enum),          ConfigurationCell(Text,"bunchnum", bunchnum),
        ConfigurationCell(E,"r1", _r1),                 ConfigurationCell(E,"r2", _r2), 
        ConfigurationCell(Text,"epath", epath),         ConfigurationCell(Text,"bpath", bpath), 
        ConfigurationCell(Text,"cpath", cpath),         ConfigurationCell(Text,"ppath", ppath), 
        ConfigurationCell(Text,"multh",multh),          ConfigurationCell(Text,"thcount",thcount),      
        ConfigurationCell(B,"magrotation", magrotation),ConfigurationCell(B,"addmagnet", addmagnet),    
        ConfigurationCell(Text,"output", output)};

    const std::vector<std::string> ConfigurationFrame::label_texts =   {"emax", "ein", "targeten", "freq", "phaselag", "starttime",
                                                                        "endtime", "dt", "guntime", "gunperiod", "enum", "bunchnum",
                                                                        "r1", "r2", "epath", "bpath", "cpath", "ppath",
                                                                        "multh", "thcount", "magrotation", "addmagnet", "output"};


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