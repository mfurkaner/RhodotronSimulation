#ifndef GUI_RENDERER_H
#define GUI_RENDERER_H

#include "../../SIM/basic/vector.h"
#include "../data/gui_sim_dataprovider.h"
#include "render_settings.h"
#include <vector>
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TColor.h"
#include "TPaveText.h"
#include "TTimer.h"
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

        data::DataProvider* _dataProvider; 
        TRootEmbeddedCanvas *canvas_to_render_in;

        float _ein = 0.04;
        float _targetEnergy = 1.5;

        std::vector<TEllipse*> electrons;
        std::vector<TArrow*> rfFieldArrows;
        std::vector<TEllipse*> posBField;

        TPaveText* _time_legend;
        TPaveText* _legend;
        std::vector<TEllipse*> _legend_electron_energy_samples;

        TEllipse* _cavity_r1;
        TEllipse* _cavity_r2;

        TTimer *timer;
        bool render_ready = false;

        bool _save_gif = false;
        const char* _temp_gif_frames_path = "temp/gif_frames";

        void _renderElectrons();
        void _renderEField();
        void _renderBField();
        void _renderLegend();
        void _renderCavity();

        void _updateElectrons(float time);
        void _updateEField(int log_index);
        void _updateBField(int log_index);
        void _updateLegend(int log_index);
        void _updateLegend(float time);
        void _updateCavity();

        int _indexFromTime(float time);
    public:
        Renderer(){timer = new TTimer(1);}
        ~Renderer(){timer->TurnOff(); delete timer;}

        void SetDataProvider(data::DataProvider* dp){_dataProvider = dp;}
        void SetTargetEnergy(float targetEn) { _targetEnergy = targetEn;}
        void SetCanvasToRender(TRootEmbeddedCanvas* canvas){ canvas_to_render_in = canvas;}

        void Render();

        void GoToTime(float time);
        void DrawTimeStamp(float time);

        void clear();
        void RunRendered();
        void SaveGif();
        void Save();
        void iterate();
    };

}
#endif