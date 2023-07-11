#ifndef GUI_SWEEP_BASE_SUBFRAME_H
#define GUI_SWEEP_BASE_SUBFRAME_H

#include "TGNumberEntry.h"
#include "TGSlider.h"
#include "TRootEmbeddedCanvas.h"


#include "../../gui_frames_common.h"
#include "../gui_sweep_frame.h"
#include "../../../analyzer/definitions.h"
#include "../../../comm/sim_server.h"
#include "../../../data/gui_sim_dataprovider.h"
#include "../../config_frame/gui_config_frame.h"

namespace RhodotronSimulatorGUI::frames::subframes{

    class SweepControlFrame{
    protected:
        TGFrame* parent;

        TGHProgressBar* progressBar;
        TRootEmbeddedCanvas* canvas;

        Analysis::Analyzer* analyzer;
        GUISimulationHandler* sim_handler;
        data::DataProvider* dataProvider;

        ConfigurationFrame* config_frame;    
    public:
        SweepControlFrame(const TGWindow* p, Analysis::Analyzer* analyzer_, GUISimulationHandler* sim_handler_, data::DataProvider* dataProvider_, ConfigurationFrame* config_frame_);


        TGHProgressBar* GetProgressBar(){return progressBar;}
    };

    SweepControlFrame::SweepControlFrame(const TGWindow* p, Analysis::Analyzer* analyzer_, 
    GUISimulationHandler* sim_handler_, data::DataProvider* dataProvider_, ConfigurationFrame* config_frame_)
    {
        parent = (TGFrame*)p;
        analyzer = analyzer_;
        sim_handler = sim_handler_;
        dataProvider = dataProvider_;
        config_frame = config_frame_;

    }


    class SweepControlVerticalFrame : public TGVerticalFrame, public SweepControlFrame{
    public:
        SweepControlVerticalFrame(const TGWindow* p, UInt_t w, UInt_t h, 
            Analysis::Analyzer* analyzer_, GUISimulationHandler* sim_handler_, data::DataProvider* dataProvider_, ConfigurationFrame* config_frame_);

        virtual void OnNavigatedTo() = 0;
        virtual void OnNavigatedFrom() = 0;
    };

    SweepControlVerticalFrame::SweepControlVerticalFrame(const TGWindow* p, UInt_t w, UInt_t h, 
        Analysis::Analyzer* analyzer_, GUISimulationHandler* sim_handler_, data::DataProvider* dataProvider_, ConfigurationFrame* config_frame_)
        : TGVerticalFrame(p, w, h), SweepControlFrame(p, analyzer_, sim_handler_, dataProvider_, config_frame_)
    {
        
        // Setup the progressbar
        progressBar = new TGHProgressBar(this, TGProgressBar::EBarType::kFancy, 250);
        progressBar->SetHeight(100);
        progressBar->SetMin(0);
        progressBar->SetMax(0b00011111);
        progressBar->ShowPosition();
        progressBar->SetBarColor("lightblue");

        // Setup the canvas
        canvas = new TRootEmbeddedCanvas("sweep", this, 500, 500);
    }



    class SweepControlHorizontalFrame : public TGHorizontalFrame, public SweepControlFrame{
    public:
        SweepControlHorizontalFrame(const TGWindow* p, UInt_t w, UInt_t h, 
            Analysis::Analyzer* analyzer_, GUISimulationHandler* sim_handler_, data::DataProvider* dataProvider_, ConfigurationFrame* config_frame_);

        virtual void OnNavigatedTo() = 0;
        virtual void OnNavigatedFrom() = 0;
    };


    SweepControlHorizontalFrame::SweepControlHorizontalFrame(const TGWindow* p, UInt_t w, UInt_t h, 
        Analysis::Analyzer* analyzer_, GUISimulationHandler* sim_handler_, data::DataProvider* dataProvider_, ConfigurationFrame* config_frame_)
        : TGHorizontalFrame(p, w, h), SweepControlFrame(p, analyzer_, sim_handler_, dataProvider_, config_frame_)
    {
        
        // Setup the progressbar
        progressBar = new TGHProgressBar(this, TGProgressBar::EBarType::kFancy, 250);
        progressBar->SetHeight(100);
        progressBar->SetMin(0);
        progressBar->SetMax(0b00011111);
        progressBar->ShowPosition();
        progressBar->SetBarColor("lightblue");

        // Setup the canvas
        canvas = new TRootEmbeddedCanvas("output", this, 600, 500);
    }

}

#endif