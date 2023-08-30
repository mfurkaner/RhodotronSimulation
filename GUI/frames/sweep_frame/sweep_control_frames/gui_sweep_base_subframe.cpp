#include "gui_sweep_base_subframe.h"

//namespace RhodotronSimulatorGUI::frames::subframes{


    SweepControlFrame::SweepControlFrame(const TGWindow* p, Analyzer* analyzer_, 
    GUISimulationHandler* sim_handler_, DataProvider* dataProvider_, ConfigurationFrame* config_frame_)
    {
        parent = (TGFrame*)p;
        analyzer = analyzer_;
        sim_handler = sim_handler_;
        dataProvider = dataProvider_;
        config_frame = config_frame_;

        progressBar_mutex = std::make_shared<std::mutex>();
    }

    SweepControlVerticalFrame::SweepControlVerticalFrame(const TGWindow* p, UInt_t w, UInt_t h, 
        Analyzer* analyzer_, GUISimulationHandler* sim_handler_, DataProvider* dataProvider_, ConfigurationFrame* config_frame_)
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

    SweepControlHorizontalFrame::SweepControlHorizontalFrame(const TGWindow* p, UInt_t w, UInt_t h, 
        Analyzer* analyzer_, GUISimulationHandler* sim_handler_, DataProvider* dataProvider_, ConfigurationFrame* config_frame_)
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

//}