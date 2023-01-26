#ifndef GUI_RENDERER_H
#define GUI_RENDERER_H

#include "../../SIM/basic/vector.h"
#include "render_settings.h"
#include <vector>
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TColor.h"
#include <iostream>

namespace RhodotronSimulatorGUI::renderer{

    struct ElectronSnapshot{
        float time;
        float energy;
        vector3d position;
        vector3d velocity;
    };

    struct ElectronLog{
        std::vector<ElectronSnapshot> time_slices;
    };

    struct RFPoint{
        vector3d position;
        vector3d field;
        float magnitude;
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
        TPaveText* _time_legend;
        TPaveText* _legend;
        std::vector<TEllipse*> _legend_electron_energy_samples;
        uint32_t _enum;
        uint32_t _bnum;
        float _ein = 0.04;
        float _targetEnergy = 1.5;
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

        bool _save_gif = false;
        const char* _temp_gif_frames_path = "temp/gif_frames";

        void _fillElectrons();
        void _fillEField();
        void _fillBField();

        void _renderElectrons();
        void _renderEField();
        void _renderBField();
        void _renderLegend();

        void _updateElectrons(float time);
        void _updateEField(int log_index);
        void _updateBField(int log_index);
        void _updateLegend(int log_index);

        int _indexFromTime(float time);
    public:
        Renderer(){timer = new TTimer(1);}
        ~Renderer(){timer->TurnOff(); delete timer;}

        void SetEnum(int _enum_) {_enum = _enum_;}
        void SetBnum(int _bnum_) {_bnum = _bnum_;}
        void SetTargetEnergy(float targetEn) { _targetEnergy = targetEn;}

        void fillLogs();
        void Render(TRootEmbeddedCanvas *canvas);

        void GoToTime(float time);
        void DrawTimeStamp(float time);

        void clear();
        void RunRendered();
        void SaveGif();
        void Save();
        void iterate();
    };

}
std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::ElectronSnapshot& e_snapshot); 
std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::RFPoint& rf_point); 
std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::RFSnapshot& rf_snapshot); 


#endif