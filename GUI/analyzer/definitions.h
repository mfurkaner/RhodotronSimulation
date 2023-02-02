#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "../data/gui_sim_dataprovider.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TColor.h"
#include <vector>



namespace RhodotronSimulatorGUI::Analysis{


    struct ePerformance{

    };


    class Analyzer{
        data::DataProvider* _dataProvider;
        TRootEmbeddedCanvas* _canvas;

        TH1I* hist;
        TPaveText* _time_legend;

        TGraph* graph;
        void _renderLegend();
        void _updateLegend(float time);

    public:

        Analyzer();

        void SetDataProvider(data::DataProvider* pDataProvider) { _dataProvider = pDataProvider; }
        void SetCanvas(TRootEmbeddedCanvas* pCanvas) { _canvas = pCanvas; }

        void DrawEnergyDistribution(float time = -1);
        void DrawEvsT(int b_index, int e_intex, double tmin, double tmax = -1);

        int TimeIndexFromTime(int e_index, float time);
    };


    Analyzer::Analyzer(){
        hist = new TH1I("Electron Energies", "Electron Energy Distribution", 100, 0, 3);
        graph = new TGraph(1);
    }

    void Analyzer::_renderLegend(){
        double x1, y1, x2, y2;
        _canvas->GetCanvas()->GetRange(x1, y1, x2, y2);
        double timeX1 = x1 + (x2 - x1)*0.05;
        double timeX2 = x1 + (x2 - x1)*0.23;
        double timeY1 = y1 + (y2 - y1)*0.05;
        double timeY2 = y1 + (y2 - y1)*0.1;
        _time_legend = new TPaveText(timeX1,timeY1, timeX2, timeY2);
        char temp[50];
        snprintf(temp, 50, "t = %.1fns", 0.0);
        _time_legend->AddText(temp);
        _time_legend->Draw();
    }

    void Analyzer::_updateLegend(float time){
        char temp[50];
        snprintf(temp, 50, "t = %.1fns", time);
        _time_legend->Clear();
        _time_legend->AddText(temp);
        _time_legend->Draw();  
    }
        

    void Analyzer::DrawEnergyDistribution(float time){
        auto e_logs = _dataProvider->GetElectrons();

        hist->Reset();
        hist->SetBarWidth(1); 

        double energy_step = 3.0/100;
        double min = e_logs[0].time_slices.back().energy;
        double max = 0;
        int minbin, maxbin, count = 0;

        for(int i = 0; i < e_logs.size(); i++){
            int bin = 1;
            int t = TimeIndexFromTime(i, time);
            if( t == -1){
                continue;
            }
            
            for ( bin = 1; bin <= 100; bin++){
                if ( e_logs[i].time_slices[t].energy < energy_step * bin){
                    break;
                }
            }
            if ( min > e_logs[i].time_slices[t].energy){
                min = e_logs[i].time_slices[t].energy;
                minbin = bin;
            }
            if ( max < e_logs[i].time_slices[t].energy){
                max = e_logs[i].time_slices[t].energy;
                maxbin = bin;
            }
            hist->AddBinContent(bin);
            count++;
        }
        _canvas->Clear();
        _canvas->GetCanvas()->cd();

        hist->GetXaxis()->SetRange(minbin/2, maxbin + 1);
        char t[10];
        snprintf(t, 10, "%.1fns", time);
        std::string title = "Electron Energy Distribution at t = ";
        title += t;
        hist->SetTitle(title.c_str());
        hist->GetXaxis()->SetTitle("E(MeV)");
        hist->GetXaxis()->CenterTitle();
        hist->GetYaxis()->SetTitle("Count");
        hist->GetYaxis()->CenterTitle();
        hist->SetEntries(count);
        hist->SetFillColor(TColor::GetColor(132, 193, 163));
        hist->Draw("BAR2");

        //_renderLegend();
        //_updateLegend(time);

        _canvas->GetCanvas()->Modified();
        _canvas->GetCanvas()->Update();
    }

    void Analyzer::DrawEvsT(int b_index, int e_index, double tmin, double tmax){
        auto e_log = _dataProvider->GetElectrons();
        int index = 0;

        for(; index < e_log.size(); index++){
            if ( b_index == e_log[index].b_index && e_index == e_log[index].e_index){
                break;
            }
        }
        if ( index >= e_log.size() ) return;

        delete graph;
        graph = new TGraph(1);

        for(int t = 0; t < e_log[index].time_slices.size(); t++){
            double time = e_log[index].time_slices[t].time;
            double E = e_log[index].time_slices[t].energy;
            graph->AddPoint(time, E);
        }

        _canvas->GetCanvas()->cd(1);
        _canvas->GetCanvas()->Clear();


        std::string title = "Electron " + std::to_string(e_index) + " in bunch "  + std::to_string(b_index); 
        graph->SetTitle(title.c_str());
        graph->GetXaxis()->SetTitle("t(ns)");
        graph->GetXaxis()->CenterTitle();
        graph->GetYaxis()->SetTitle("E(MeV)");
        graph->GetYaxis()->CenterTitle();
        if(tmax != -1){
            graph->GetXaxis()->SetRangeUser(tmin, tmax);
        }

        graph->Draw();

        _canvas->GetCanvas()->Modified();
        _canvas->GetCanvas()->Update();
    }


    int Analyzer::TimeIndexFromTime(int e_index, float time){
        auto e_logs = _dataProvider->GetElectrons();

        if ( e_logs.size() <= e_index ) return -1;
        else if (time == -1){
            return e_logs[e_index].time_slices.size() - 1;
        }

        int t = 0;

        for (; t < e_logs[e_index].time_slices.size() ;  t++){
            if ( e_logs[e_index].time_slices[t].time == time){
                return t;
            }
        }
        return -1;
    }

};

#endif