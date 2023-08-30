#ifndef GUI_t_vs_E_DIST_H
#define GUI_t_vs_E_DIST_H

#include "TGNumberEntry.h"
#include "TGDoubleSlider.h"


#include "../../gui_frames_common.h"

//namespace RhodotronSimulatorGUI::frames::subframes{

    class TvsEControlFrame : public TGVerticalFrame{
        const TGWindow* parent;

        TGLabel* select_time_label;
        TGLabel* e_label;
        TGLabel* b_label;
        TGNumberEntry* e_index;
        TGNumberEntry* b_index;

        TGDoubleHSlider* time_interval_select;

        double _start_time;
        double _end_time;
        
    public:
        TvsEControlFrame(const TGWindow* p, UInt_t w, UInt_t h);
        void Setup();

        void SetTimeInterval(double start_time, double end_time);
        void SetEnum(int _enum);
        void SetBnum(int bnum);
        int GetBIndex();
        int GetEIndex();
        std::pair<double,double> GetTimeIntervalToDraw();
    };


//}

#endif