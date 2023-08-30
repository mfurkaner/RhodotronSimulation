#ifndef GUI_SWEEP_BASE_SUBFRAME_H
#define GUI_SWEEP_BASE_SUBFRAME_H

#include "TGNumberEntry.h"
#include "TGSlider.h"
#include "TRootEmbeddedCanvas.h"


#include "../../gui_frames_common.h"
#include "../../../analyzer/gui_analyzer.h"
#include "../../../comm/sim_server.h"
#include "../../../data/gui_sim_dataprovider.h"
#include "../../config_frame/gui_config_frame.h"

//namespace RhodotronSimulatorGUI::frames::subframes{

    class SweepControlFrame{
    protected:
        TGFrame* parent;

        TGHProgressBar* progressBar;
        std::shared_ptr<std::mutex> progressBar_mutex;
        TRootEmbeddedCanvas* canvas;

        Analyzer* analyzer;
        GUISimulationHandler* sim_handler;
        DataProvider* dataProvider;

        ConfigurationFrame* config_frame;    
    public:
        SweepControlFrame(const TGWindow* p, Analyzer* analyzer_, GUISimulationHandler* sim_handler_, DataProvider* dataProvider_, ConfigurationFrame* config_frame_);


        TGHProgressBar* GetProgressBar(){return progressBar;}
    };
    class SweepControlVerticalFrame : public TGVerticalFrame, public SweepControlFrame{
    public:
        SweepControlVerticalFrame(const TGWindow* p, UInt_t w, UInt_t h, 
            Analyzer* analyzer_, GUISimulationHandler* sim_handler_, DataProvider* dataProvider_, ConfigurationFrame* config_frame_);

        virtual void OnNavigatedTo() = 0;
        virtual void OnNavigatedFrom() = 0;
    };

    class SweepControlHorizontalFrame : public TGHorizontalFrame, public SweepControlFrame{
    public:
        SweepControlHorizontalFrame(const TGWindow* p, UInt_t w, UInt_t h, 
            Analyzer* analyzer_, GUISimulationHandler* sim_handler_, DataProvider* dataProvider_, ConfigurationFrame* config_frame_);

        virtual void OnNavigatedTo() = 0;
        virtual void OnNavigatedFrom() = 0;
    };


//}

#endif