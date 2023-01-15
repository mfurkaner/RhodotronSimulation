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
        std::vector<TEllipse*> es;
        std::vector<TArrow*> rfFieldArrows;
        RFLog _rf;
        StaticMagneticFieldLog _magnets;
        TTimer *timer;
        bool render_ready = false;

        void _fillElectrons();
        void _fillRf();
        void _fillMagnets();
        void _renderElectrons();
        void _renderRF();
        void _renderMagnets();
    public:
        Renderer(){timer = new TTimer(10);}
        ~Renderer(){timer->TurnOff(); delete timer;}

        void fillLogs();
        void render(TRootEmbeddedCanvas *canvas);

        void GoToTime(float time);

        void clear();
        void run_rendered();
        void iterate();
    };
}
std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::ElectronSnapshot& e_snapshot); 
std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::RFPoint& rf_point); 
std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::RFSnapshot& rf_snapshot); 
#endif