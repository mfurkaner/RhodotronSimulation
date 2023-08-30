#ifndef GUI_SWEEP_PHASELAG_H
#define GUI_SWEEP_PHASELAG_H

#include "TGNumberEntry.h"
#include "TGSlider.h"


#include "../../gui_frames_common.h"
#include "gui_sweep_base_subframe.h"
#include "../../../data/gui_data_types.h"

using namespace RhodotronSimulatorGUI::data::PhaseLagSweep;

namespace RhodotronSimulatorGUI::frames::subframes{

    struct PhaseLagSweepWorkerArguments;

    class PhaseLagSweepControlFrame : public SweepControlVerticalFrame{

        TGLabel* current_phase;

        TGLabel* phase_lag_sweep_start_label;
        TGNumberEntry* phase_lag_sweep_start;
        TGLabel* phase_lag_sweep_end_label;
        TGNumberEntry* phase_lag_sweep_end;

        TGTextButton* _sweep_button;
        TGTextButton* _stop_button;

        TGTextButton* _draw_phase_vs_muE_button;
        TGTextButton* _draw_phase_vs_sigmaE_button;
        TGTextButton* _draw_phase_vs_sigmaR_button;

        PhaseLagGraphType _active_graph = None;
        TTimer* _draw_timer;

        TGraph* _phase_vs_muE_graph;
        TGraph* _phase_vs_sigmaE_graph;
        TGraph* _phase_vs_sigmaR_graph;
        
        std::shared_ptr<std::mutex> _phase_lag_data_mutex;
        std::vector<PhaseLagSweepData> phaselagData;

        std::thread* worker = NULL;
        std::shared_ptr<bool> _worker_terminate;
        std::shared_ptr<bool> _data_updated;
        std::shared_ptr<std::mutex> _data_updated_mutex;
        bool _sweeping_mode_active = false;
    public:
        PhaseLagSweepControlFrame(const TGWindow* p, UInt_t w, UInt_t h, 
            Analysis::Analyzer* analyzer_, GUISimulationHandler* sim_handler_, data::DataProvider* dataProvider_, ConfigurationFrame* config_frame_);

        
        void ClearGraphs();
        void DrawPhaseLagvsAverageE();
        void DrawPhaseLagvsStdE();
        void DrawPhaseLagvsStdR();

        void DrawActiveGraph();
        void DrawActiveGraphIfThereIsUpdate();

        void PhaselagSweepRequested();
        void PhaselagSweepStopRequested();
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
        std::shared_ptr<std::mutex> dataMutex;

        std::shared_ptr<bool> terminate;
        
        std::shared_ptr<bool> dataUpdated;
        std::shared_ptr<std::mutex> dataUpdatedMutex;
    };


}

#endif