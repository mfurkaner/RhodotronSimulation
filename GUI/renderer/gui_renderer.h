#ifndef GUI_RENDERER_H
#define GUI_RENDERER_H

#include "../../SIM/basic/vector.h"
#include <vector>
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include <iostream>

namespace RhodotronSimulatorGUI::renderer{

    struct ElectronSnapshot{
        float time;
        double energy;
        vector3d position;
        vector3d velocity;
    };

    struct ElectronLog{
        std::vector<ElectronSnapshot> time_slices;
    };

    struct RFPoint{
        vector3d position;
        vector3d field;
        double magnitude;
    };

    struct RFSnapshot{
        float time;
        std::vector<RFPoint> field;
    };

    struct RFLog{
        std::vector<RFSnapshot> time_slices;
    };

    struct StaticMagneticFieldLog{
        std::vector<std::pair<float, float>> positive_positions;
    };

    class Renderer{
        TRootEmbeddedCanvas *canvas;
        uint32_t _enum;
        std::string _elog_path = "xy/paths/";
        std::string _rflog_path = "xy/rf.txt";
        std::string _mlog_path = "xy/magnet.txt";

        std::vector<ElectronLog> _electrons_log;
        RFLog _rf;
        StaticMagneticFieldLog _magnets;

        std::vector<TEllipse*> electrons;
        std::vector<TArrow*> rfFieldArrows;
        std::vector<TEllipse*> posBField;

        TTimer *timer;
        bool render_ready = false;

        void _fillElectrons();
        void _fillEField();
        void _fillBField();

        void _renderElectrons();
        void _renderEField();
        void _renderBField();

        void _updateElectrons(int log_index);
        void* _updateElectronsTH(void* log_intex);

        void _updateEField(int log_index);
        void* _updateEFieldTH(void* log_intex);

        void _updateBField(int log_index);
        void* _updateBFieldTH(void* log_intex);

        int _indexFromTime(float time);
    public:
        Renderer(){timer = new TTimer(10);}
        ~Renderer(){timer->TurnOff(); delete timer;}

        void fillLogs();
        void Render(TRootEmbeddedCanvas *canvas);

        void GoToTime(float time);

        void clear();
        void RunRendered();
        void iterate();
    };
}
std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::ElectronSnapshot& e_snapshot); 
std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::RFPoint& rf_point); 
std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::RFSnapshot& rf_snapshot); 


#endif