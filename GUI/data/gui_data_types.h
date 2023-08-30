#ifndef GUI_DATA_TYPES_H
#define GUI_DATA_TYPES_H

namespace RhodotronSimulatorGUI::data{

    namespace PhaseLagSweep{
        struct PhaseLagSweepData{
            float phase;
            float muE;
            float sigmaE;
            float sigmaR;
        };

        enum PhaseLagGraphType{
            muE_vs_phase,
            sE_vs_phase,
            sR_vs_phase,
            None
        };
    }

}
#endif