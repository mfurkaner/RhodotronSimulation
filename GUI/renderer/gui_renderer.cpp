#include "gui_renderer.h"
#include "../../SIM/basic/vector.cpp"
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include "TEllipse.h"
#include "TArrow.h"
#include "TTimer.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TPaveText.h"
#include "TLegend.h"
#include <thread>



namespace RhodotronSimulatorGUI::renderer{

#pragma region RENDER_FILLED

    Color_t EnergyGradient(float E, float Emax){
        if( E <= 0 )
            return TColor::GetColor(10,10,10);

        const static Color_t color_map[10]{
            (Color_t)TColor::GetColor(11, 22, 103),
            (Color_t)TColor::GetColor(67, 16, 110),
            (Color_t)TColor::GetColor(105, 0, 112),
            (Color_t)TColor::GetColor(139, 0, 109),
            (Color_t)TColor::GetColor(170, 0, 102),
            (Color_t)TColor::GetColor(197, 0, 92),
            (Color_t)TColor::GetColor(219, 0, 78),
            (Color_t)TColor::GetColor(236, 34, 62),
            (Color_t)TColor::GetColor(247, 70, 41),
            (Color_t)TColor::GetColor(253, 101, 0)

/*
            (Color_t)TColor::GetColor(252, 112, 50),
            (Color_t)TColor::GetColor(236, 100, 52),
            (Color_t)TColor::GetColor(219, 89, 54),
            (Color_t)TColor::GetColor(203, 77, 56),
            (Color_t)TColor::GetColor(187, 66, 56),
            (Color_t)TColor::GetColor(171, 54, 57),
            (Color_t)TColor::GetColor(156, 43, 57),
            (Color_t)TColor::GetColor(140, 31, 56),
            (Color_t)TColor::GetColor(125, 17, 56),
            (Color_t)TColor::GetColor(110, 0, 55)*/

        };
        float step = Emax / 10;
        int times = E/step;
        int index = times >= 10 ? 9 : times;

        return color_map[index];
    }

    Color_t EGradient(float E){
        const static Color_t color_map[10]{
            /*
            (Color_t)TColor::GetColor(27, 2, 222),
            (Color_t)TColor::GetColor(47, 0, 205),
            (Color_t)TColor::GetColor(57, 0, 188),
            (Color_t)TColor::GetColor(62, 0, 172),
            (Color_t)TColor::GetColor(65, 0, 156),
            (Color_t)TColor::GetColor(65, 0, 141),
            (Color_t)TColor::GetColor(64, 0, 127),
            (Color_t)TColor::GetColor(62, 1, 113),
            (Color_t)TColor::GetColor(59, 3, 99),
            (Color_t)TColor::GetColor(55, 6, 86)*/
            
            
            (Color_t)TColor::GetColor(252, 112, 50),
            (Color_t)TColor::GetColor(236, 100, 52),
            (Color_t)TColor::GetColor(219, 89, 54),
            (Color_t)TColor::GetColor(203, 77, 56),
            (Color_t)TColor::GetColor(187, 66, 56),
            (Color_t)TColor::GetColor(171, 54, 57),
            (Color_t)TColor::GetColor(156, 43, 57),
            (Color_t)TColor::GetColor(140, 31, 56),
            (Color_t)TColor::GetColor(125, 17, 56),
            (Color_t)TColor::GetColor(110, 0, 55)

        };
        float step = 0.96 / 10;
        int times = E/step;
        int index = times >= 10 ? 9 : times;

        return color_map[index];
    }

    void Renderer::_renderElectrons(){
        auto e_logs = _dataProvider->GetElectrons();
        for(int i = 0; i < e_logs.size(); i++){
            TEllipse* point = new TEllipse( e_logs[i].time_slices.at(0).position.X(),//0.5 + _electrons_log[i].time_slices.at(0).position.X()/3,
                                            e_logs[i].time_slices.at(0).position.Y(),//0.5 + _electrons_log[i].time_slices.at(0).position.Y()/3,
                                            SIZE_e_PIXEL);
            //point->Draw();
            point->SetFillColor(EnergyGradient(e_logs[i].time_slices.at(0).energy, _targetEnergy));
            point->SetLineStyle(0);
            //point->SetLineColor(5);
            electrons.push_back(point);
        }
    }
    
    void Renderer::_renderEField(){
        auto rf_log = _dataProvider->GetEField();
        arrowSizeMultiplier = SIZE_E_ARROW_MULTIPLIER/rf_log.RF_max;

        std::cout << "RF max (MV/m) = " << rf_log.RF_max 
                  << " : arrowSizeMultiplier = " << arrowSizeMultiplier << std::endl;

        if(rf_log.time_slices.size() > 0){
            float x1,y1,x2,y2;

            for(int i = 0; i < rf_log.time_slices.at(0).field.size() ; i++){
                auto point = rf_log.time_slices.at(0).field.at(i);

                point.field *= arrowSizeMultiplier;
                //point.position /= 3;

                x1 = point.position.X();
                y1 = point.position.Y();
                x2 = x1 + point.field.X();
                y2 = y1 + point.field.Y();
                TArrow* rfArrow = new TArrow(x1, y1, x2, y2, 0.005);
                rfArrow->SetLineColor(EGradient(point.magnitude));

                if( point.magnitude != 0.0 ){
                    rfArrow->Draw();
                }
                rfFieldArrows.push_back(rfArrow);
            }
        }
    }

    void Renderer::_renderBField(){
        auto b_log = _dataProvider->GetStaticBField();
        if( b_log.non_zero_positions.size() <= 0 ){
            return;
        }

        for ( int i = 0; i < b_log.non_zero_positions.size(); i++ ){
            TEllipse* point = new TEllipse( b_log.non_zero_positions[i].first,
                                            b_log.non_zero_positions[i].second,
                                            SIZE_B_PIXEL, SIZE_B_PIXEL);
            point->Draw();
            point->SetFillColor(COLOR_B_PIXEL_STATIC);
            point->SetLineColor(COLOR_B_PIXEL_STATIC);
            posBField.push_back(point);
        }
    }

    void Renderer::_renderLegend(){
        double x1, y1, x2, y2;
        canvas_to_render_in->GetCanvas()->GetRange(x1, y1, x2, y2);
        double timeX1 = x1 + (x2 - x1)*0.05;
        double timeX2 = x1 + (x2 - x1)*0.23;
        double timeY1 = y1 + (y2 - y1)*0.05;
        double timeY2 = y1 + (y2 - y1)*0.1;
        _time_legend = new TPaveText(timeX1,timeY1, timeX2, timeY2);
        char temp[50];
        snprintf(temp, 50, "t = %.1fns", 0.0);
        _time_legend->AddText(temp);
        _time_legend->Draw();
/*
        TEllipse* b_sample = new TEllipse();
        b_sample->SetR1(SIZE_B_PIXEL);
        b_sample->SetR2(SIZE_B_PIXEL);
        b_sample->Draw();
        b_sample->SetFillColor(COLOR_B_PIXEL_STATIC);
        b_sample->SetLineColor(COLOR_B_PIXEL_STATIC);*/

        auto E_step = _targetEnergy/10;

        std::string legend_energy_text = "";


        for(int i = 0; i < 11; i++){
            char temp[20];
            snprintf(temp, 20, "%.1f", E_step*(i));
            legend_energy_text += temp;
            if( i+1 < 11){
                legend_energy_text += "      ";
            }
        }

        double legendX1 = x1 + (x2 - x1)*0.05;
        double legendX2 = x1 + (x2 - x1)*0.95;
        double legendY1 = y1 + (y2 - y1)*0.86;
        double legendY2 = y1 + (y2 - y1)*0.98;

        _legend = new TPaveText(legendX1, legendY1, legendX2, legendY2);
        _legend->AddText("Electron Energy Scale (MeV)");
        auto first_line = _legend->AddLine(.2, .66, .8, .66);
        first_line->SetLineStyle(kDashed);

        _legend->AddText("");
        _legend->AddLine(.0, .33, 1., .33);

        _legend->AddText(legend_energy_text.c_str());

        _legend->Draw();

        std::vector<TEllipse*> e_energy_legend_sample;

        for(int i = 0; i < 11 ; i ++){
            double sampleXstep = (x2 - x1)*0.08;
            double sampleX = x1 + (x2 - x1)*0.1;
            double sampleY = x1 + (x2 - x1)*0.92;
            double sampleR = (y2 - y1)*0.01;
            TEllipse* sample = new TEllipse(i*sampleXstep + sampleX, sampleY, sampleR);
            sample->SetFillColor(EnergyGradient(E_step*(i), _targetEnergy));
            _legend_electron_energy_samples.push_back(sample);
            sample->Draw();
        }
    }

    void Renderer::_renderCavity(){

        double r1 = 0.188, r2 = 0.753;
        _cavity_r1 = new TEllipse(0, 0, r1);
        _cavity_r1->SetFillColorAlpha(canvas_to_render_in->GetCanvas()->GetFillColor(), 1);
        _cavity_r1->SetLineStyle(kDashed);
        _cavity_r1->SetLineWidth(2);

        _cavity_r2 = new TEllipse(0, 0, r2);
        _cavity_r2->SetFillColorAlpha(kBlue, 0.2);
        _cavity_r2->SetLineStyle(kDashed);
        _cavity_r2->SetLineWidth(2);
        
        _cavity_r2->Draw();
        _cavity_r1->Draw();
    }

    void Renderer::Render(){
        canvas_to_render_in->GetCanvas()->Clear();
        canvas_to_render_in->DrawBorder();

        // setup the cavity
        _renderCavity();

        // setup the electrons
        _renderElectrons();

        // setup E field
        _renderEField();

        // setup B field
        _renderBField();

        // setup legend
        _renderLegend();

        canvas_to_render_in->GetCanvas()->Modified();
        canvas_to_render_in->GetCanvas()->Update();

        render_ready = true;
        //gSystem->Unlink("gui_output.gif");
    }
#pragma endregion RENDER_FILLED

    void Renderer::_updateElectrons(float time){
        auto e_logs = _dataProvider->GetElectrons();
        for(int j = 0; j < electrons.size(); j++){
            int i = 0;
            for( ; i < e_logs[j].time_slices.size(); i++){
                if ( e_logs[j].time_slices[i].time == time ){
                    break;
                }
            }
            if (i < e_logs[j].time_slices.size()){
                electrons[j]->SetX1(e_logs[j].time_slices[i].position.X());
                electrons[j]->SetY1(e_logs[j].time_slices[i].position.Y());
                electrons[j]->SetFillColor(EnergyGradient(e_logs[j].time_slices[i].energy, 1.5));
                electrons[j]->Draw();
            }
        }
    }

    void Renderer::_updateEField(int log_index){
        auto rf_log = _dataProvider->GetEField();
        if (log_index < rf_log.time_slices.size()){

            for(int j = 0; j < rf_log.time_slices.at(log_index).field.size() ; j++){
                auto point = rf_log.time_slices.at(log_index).field.at(j);

                if( point.magnitude == 0 )
                    continue;

                point.field *= arrowSizeMultiplier;
                //point.position /= 3;

                float x1,y1,x2,y2;
                x1 = point.position.X();
                y1 = point.position.Y();
                x2 = x1 + point.field.X();
                y2 = y1 + point.field.Y();

                rfFieldArrows[j]->SetLineColor(EGradient(point.magnitude));
                rfFieldArrows[j]->DrawArrow(x1, y1, x2, y2, 0.005);
            }

        }
        else{
            std::cerr << "An index bigger than rf.time_slices is requested : " << log_index <<  " last time was : " << rf_log.time_slices[rf_log.time_slices.size()-1].time << std::endl;
        }
    }

    void Renderer::_updateBField(int log_index){
        // STATIC FOR NOW!!! 
        for(int i = 0; i < posBField.size(); i++){
            posBField[i]->Draw();
        }
    }

    void Renderer::_updateLegend(int log_index){
        auto e_logs = _dataProvider->GetElectrons();
        auto rf_log = _dataProvider->GetEField();
        float time = 0;
        if(e_logs.size() != 0 && e_logs[0].time_slices.size() > log_index){
            time = e_logs[0].time_slices.at(log_index).time;
        }
        else if (rf_log.time_slices.size() > log_index){
            time = rf_log.time_slices.at(log_index).time;
        }

        char temp[50];
        snprintf(temp, 50, "t = %.1fns", time);
        _time_legend->Clear();
        _time_legend->AddText(temp);
        _time_legend->Draw();


        _legend->Draw();

        for(auto sample : _legend_electron_energy_samples){
            sample->Draw();
        }
    }

    void Renderer::_updateLegend(float time){
        char temp[50];
        snprintf(temp, 50, "t = %.1fns", time);
        _time_legend->Clear();
        _time_legend->AddText(temp);
        _time_legend->Draw();


        _legend->Draw();

        for(auto sample : _legend_electron_energy_samples){
            sample->Draw();
        }
    }

    void Renderer::_updateCavity(){
        _cavity_r2->Draw();
        _cavity_r1->Draw();
    }

    void Renderer::RunRendered(){
        if(render_ready == false || timer->IsRunning()) return;

        timer->Connect("Timeout()", "RhodotronSimulatorGUI::renderer::Renderer", this, "iterate()");			
        timer->TurnOn();
    }

    void Renderer::SaveGif(){
        if(render_ready == false || timer->IsRunning()) return;

        _save_gif = true;
        gSystem->mkdir(_temp_gif_frames_path, true);
        gSystem->Unlink("animation.gif");
        timer->Connect("Timeout()", "RhodotronSimulatorGUI::renderer::Renderer", this, "iterate()");			
        timer->TurnOn();
    }

    void Renderer::iterate(){
        static int i = 0;
        static int end_index = _dataProvider->GetElectrons()[0].time_slices.size();

        // Check if the iteration is over
        if ( i >= end_index){
            i = 0;
            timer->TurnOff();

            if (_save_gif){
                _save_gif = false;
                gSystem->Exec("convert -delay 3 -loop 1 temp/gif_frames/*.png animation.gif");
                std::string rmdir_cmd = "rm -r ";
                rmdir_cmd += _temp_gif_frames_path;
                gSystem->Exec(rmdir_cmd.c_str());
            }
            return;
        }
        
        canvas_to_render_in->GetCanvas()->Clear();

        float time = (i)*0.1;

        _updateCavity();
        _updateEField(i);
        _updateBField(i);
        _updateElectrons(time);
        _updateLegend(time);

        canvas_to_render_in->GetCanvas()->Modified();
        canvas_to_render_in->GetCanvas()->Update();

        if( _save_gif ){
            int index = i;
            std::string filename = _temp_gif_frames_path;
            filename += "/gui_output";
            filename += std::to_string(index/1000 % 10);
            filename += std::to_string(index/100 % 10);
            filename += std::to_string(index/10 % 10);
            filename += std::to_string(index % 10);
            filename += ".png";
            canvas_to_render_in->GetCanvas()->Print(filename.c_str());
        }
        
        i++;
    }

    void Renderer::clear(){
        electrons.clear();
        rfFieldArrows.clear();
        posBField.clear();
        arrowSizeMultiplier = 0;
    }

    int Renderer::_indexFromTime(float time){
        auto rf_logs = _dataProvider->GetEField();
        for(int i = 0; i < rf_logs.time_slices.size(); i++){
            if ( rf_logs.time_slices[i].time == time ){
                return i;
            }
        }

        std::cerr << "No such time : " <<  time << std::endl;
        return -1;
    }

    void Renderer::GoToTime(float time){
        int slice_index = _indexFromTime(time);

        if ( slice_index == -1 || render_ready == false)
            return;
        canvas_to_render_in->GetCanvas()->Clear();

        _updateCavity();
        _updateEField(slice_index);
        _updateBField(slice_index);
        _updateElectrons(time);
        _updateLegend(time);


        canvas_to_render_in->GetCanvas()->Modified();
        canvas_to_render_in->GetCanvas()->Update();
        
    }

    void Renderer::DrawTimeStamp(float time){
        TPaveText* pt = new TPaveText(0.05,0.05, 0.23, 0.1);
        char temp[50];
        snprintf(temp, 50, "t = %.1fns", time);
        pt->AddText(temp);
        pt->Draw();
    }
}