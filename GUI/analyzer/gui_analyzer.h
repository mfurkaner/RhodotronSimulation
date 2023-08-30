#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "../data/gui_sim_dataprovider.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TPaveText.h"
#include <vector>



//namespace RhodotronSimulatorGUI::Analysis{


    class Analyzer{
        DataProvider* _dataProvider;
        TRootEmbeddedCanvas* _canvas;

        TH1I* hist;
        TPaveText* _time_legend;

        TGraph* graph;
        void _renderLegend();
        void _updateLegend(float time);

    public:

        Analyzer();

        void SetDataProvider(DataProvider* pDataProvider) { _dataProvider = pDataProvider; }
        void SetCanvas(TRootEmbeddedCanvas* pCanvas) { _canvas = pCanvas; }

        void DrawEnergyDistribution(float time = -1);
        void DrawEvsT(int b_index, int e_intex, double tmin, double tmax = -1);

        float GetAverageE(float time = -1);
        float GetStdDevE(float time = -1);

        vector3d GetAverageR(float time = -1);
        float GetStdDevR(float time = -1);

        int TimeIndexFromTime(int e_index, float time);
    };


//}

#endif