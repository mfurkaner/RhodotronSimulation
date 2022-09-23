#ifndef GUI_CONFIG_FRAME_H
#define GUI_CONFIG_FRAME_H



#include "TSystem.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGTextView.h"
#include "TGLabel.h"
#include "TGTextEntry.h"
#include "TGLViewer.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TImage.h"
#include "TASImage.h"

#include <stdio.h>
#include <string.h>
#include <vector>
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

#include "gui_frames_common.h"


namespace RhodotronSimulatorGUI::frames{

    class ConfigurationFrame : public TGVerticalFrame{
        static const std::string config_comment;
        static const std::vector<std::string> configs;
        static const std::vector<std::string> label_texts;
        std::vector<std::shared_ptr<TGLabel>> labels;
        std::vector<std::shared_ptr<TGTextEntry>> inputs;

        const TGWindow* parent;
    
    public:

        ConfigurationFrame(const TGWindow* p, UInt_t w, UInt_t h);


        std::string GetConfigAsString();

        void LoadConfigFromFile(const std::string& configFilePath);

    };

    const std::vector<std::string> ConfigurationFrame::configs =       {"emax", "ein", "targeten", "freq", "phaselag", "starttime",
                                                                        "endtime", "dt", "guntime", "gunperiod", "enum", "bunchnum",
                                                                        "r1", "r2", "epath", "bpath", "cfield", "ppath", "starttime",
                                                                        "multh", "thcount", "magrotation", "addmagnet", "output"};

    const std::vector<std::string> ConfigurationFrame::label_texts =   {"emax", "ein", "targeten", "freq", "phaselag", "starttime",
                                                                        "endtime", "dt", "guntime", "gunperiod", "enum", "bunchnum",
                                                                        "r1", "r2", "epath", "bpath", "cfield", "ppath", "starttime",
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
                "# starttime = ns to start the simulation (ns)\n"
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
                "# cfield = path to store the settings\n"
                "# ppath = path to store electron data\n"
                "# starttime = time to start firing the gun (ns)\n"
                "# multh = enable or disable multitheading\n"
                "# thcount = set the maximum thread to be used\n"
                "# magrotation = degrees of rotation to enter each magnet \n"
                "# addmagnet = takes 3 input. (B , R, < Radial distance of center >)\n"
                "# output = output file name \n\n"};
}

#endif