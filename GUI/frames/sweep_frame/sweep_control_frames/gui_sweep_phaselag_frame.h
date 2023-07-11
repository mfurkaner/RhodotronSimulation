#ifndef GUI_SWEEP_PHASELAG_H
#define GUI_SWEEP_PHASELAG_H

#include "TGNumberEntry.h"
#include "TGSlider.h"


#include "../../gui_frames_common.h"
#include "gui_sweep_base_subframe.h"

namespace RhodotronSimulatorGUI::frames::subframes{


    struct PhaseLagSweepData{
        float phase;
        float muE;
        float sigmaE;
        float sigmaR;
    };

    struct PhaseLagSweepWorkerArguments;

    class PhaseLagSweepControlFrame : public SweepControlVerticalFrame{

        TGLabel* current_phase;

        TGLabel* phase_lag_sweep_start_label;
        TGNumberEntry* phase_lag_sweep_start;
        TGLabel* phase_lag_sweep_end_label;
        TGNumberEntry* phase_lag_sweep_end;

        TGTextButton* _sweep_button;
        TGTextButton* _draw_phase_vs_muE_button;
        TGTextButton* _draw_phase_vs_sigmaE_button;
        TGTextButton* _draw_phase_vs_sigmaR_button;

        TGraph* _phase_vs_muE_graph;
        TGraph* _phase_vs_sigmaE_graph;
        TGraph* _phase_vs_sigmaR_graph;
        
        std::vector<PhaseLagSweepData> phaselagData;

        std::thread* worker = NULL;
    public:
        PhaseLagSweepControlFrame(const TGWindow* p, UInt_t w, UInt_t h, 
            Analysis::Analyzer* analyzer_, GUISimulationHandler* sim_handler_, data::DataProvider* dataProvider_, ConfigurationFrame* config_frame_);

        void DrawPhaseLagvsAverageE();
        void DrawPhaseLagvsStdE();
        void DrawPhaseLagvsStdR();

        void PhaselagSweepRequested();
        static void PhaselagSweepWork(PhaseLagSweepWorkerArguments args);

        void SweepingModeActivate(bool active);

        void SetCurrentPhase(double phase);

        void OnNavigatedTo() override;
        void OnNavigatedFrom() override;
    };

    struct PhaseLagSweepWorkerArguments{
        int start_phase;
        int end_phase;
        Analysis::Analyzer* analyzer;
        data::DataProvider* dataProvider;
        GUISimulationHandler* sim_handler;
        ConfigurationFrame* config_frame;

        PhaseLagSweepControlFrame* owner;

        std::vector<PhaseLagSweepData>* data;
    };


}

#endif