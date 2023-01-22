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
#include <thread>

    std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::ElectronSnapshot& e_snapshot){
        std::string line;
        std::getline(stream, line);

        double x,y,z, vx, vy, vz;

        sscanf(line.c_str(), "%f,%lf,( %lf ; %lf ; %lf ),( %lf ; %lf ; %lf )", 
                    &e_snapshot.time, &e_snapshot.energy, &x, &y, &z, &vx, &vy, &vz);

        e_snapshot.position = vector3d(x, y, z);
        e_snapshot.velocity = vector3d(vx, vy, vz);

        return stream;
    }

    std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::RFPoint& rf_point){
        std::string line;
        std::getline(stream, line);

        double x,y,z,ex,ey,ez,E;

        sscanf(line.c_str(), "( %lf ; %lf ; %lf ),( %lf ; %lf ; %lf ),%lf",
                                 &x, &y, &z, &ex, &ey, &ez, &E);

        rf_point.position = vector3d(x,y,z);
        rf_point.field = vector3d(ex,ey,ez);
        rf_point.magnitude = E;

        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, RhodotronSimulatorGUI::renderer::RFPoint& rf_point){
        stream << rf_point.position << "," << rf_point.field << "," << rf_point.magnitude;
        return stream;
    }

    std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::RFSnapshot& rf_snapshot){
        RhodotronSimulatorGUI::renderer::RFPoint point;

        // TODO: guard atof
        std::string time_str;
        std::getline(stream, time_str, ',');
        rf_snapshot.time = std::atof(time_str.c_str());
        stream >> point;

        if ( stream.fail() ) { 
            return stream;
        }

        rf_snapshot.field.push_back(point);

        float time = rf_snapshot.time;
        std::streampos prevpos;
        int count = 0;
        while (!stream.eof()) {
            prevpos = stream.tellg();

            std::getline(stream, time_str, ',');

            if ( time_str.size() ){
                time = std::atof(time_str.c_str());
            }
            else{
                std::getline(stream, time_str);
                return stream;
            }

            if( stream.fail() ) {
                std::cerr << "Error: " << strerror(errno) << " time_Str : " << time_str << std::endl;
                return stream;
            }

            if( time != rf_snapshot.time ){
                stream.seekg(prevpos);
                return stream;
            }

            stream >> point;

            //if(rf_snapshot.time == 0) std::cout << "snapshot read for t=0 : " << point.position << " " << point.field << std::endl;

            rf_snapshot.field.push_back(point);
            count++;
        }

        return stream;
    }

namespace RhodotronSimulatorGUI::renderer{

#pragma region FILL_FROM_LOGS
    void Renderer::_fillElectrons(){
        //_enum = 40;
        std::cout << "Filling Electrons : " << _enum << std::endl;

        for (uint32_t i = 0; i < _enum ; i++){

            ElectronLog e;
            std::ifstream stream;
            std::string filename = _elog_path + "e" + std::to_string(i + 1) + ".txt";

            stream.open(filename, std::ios::in);

            if(stream.fail()) { continue; }

            std::getline(stream, filename);


            while (!stream.eof()) {
                ElectronSnapshot snapshot;
                stream >> snapshot;

                e.time_slices.push_back(snapshot);
            }
            _electrons_log.push_back(e);
            stream.close();

        }
    }

    void Renderer::_fillEField(){
        std::ifstream stream;
        stream.open(_rflog_path, std::ios::in);

        std::cout << _rflog_path << " opened" << std::endl;

        if( stream.fail()) { return; }
        std::string filename;
        std::getline(stream, filename);


        while (!stream.eof()) {
            RFSnapshot snapshot;
            stream >> snapshot;
            
            if ( stream.fail())
                continue; 
            
            _rf.time_slices.push_back(snapshot);

        }

        std::cout << "Rf fill is done." << std::endl;

        stream.close();
    }

    void Renderer::_fillBField(){
        std::ifstream stream;
        stream.open(_mlog_path, std::ios::in);

        if( stream.fail()) { return; }
        std::string filename;
        std::getline(stream, filename);

        int count = 0;
        while (!stream.eof()) {
            std::string line;
            std::getline(stream, line);
            if ( count % 100 == 0){
                std::pair<float, float> position;
                float x, y;

                sscanf(line.c_str(), "%f %f", &x, &y);

                position.first = x;
                position.second = y;
                
                if ( stream.fail()) { return; }

                _magnets.positive_positions.push_back(position);
            }
            count++;
        }
        stream.close();
    }

    void Renderer::fillLogs(){
        auto fill_start = std::chrono::high_resolution_clock::now();
        _fillElectrons();
        _fillEField();
        _fillBField();

        auto fill_end = std::chrono::high_resolution_clock::now();

        auto fill_time = std::chrono::duration_cast<std::chrono::milliseconds>(fill_end - fill_start);

        std::cout << "Fill took " << fill_time.count()/1000.0 << " s" << std::endl;
    }
#pragma endregion FILL_FROM_LOGS

#pragma region RENDER_FILLED
    void Renderer::_renderElectrons(){
        for(int i = 0; i < _electrons_log.size(); i++){
            TEllipse* point = new TEllipse( 0.5 + _electrons_log[i].time_slices.at(0).position.X()/3,
                                            0.5 + _electrons_log[i].time_slices.at(0).position.Y()/3,
                                            0.008, 0.008);
            point->Draw();
            point->SetFillColor(5);
            //point->SetLineColor(5);
            electrons.push_back(point);
        }
    }
    

    Color_t EGradient(double E){
        int color = 0.96/E;
        Color_t value = kOrange - 9;
        switch (color)
        {
        case 8:
            value = kOrange - 4;
            break;
        case 7:
            value = kOrange - 2;
            break;
        case 6:
            value = kOrange;
            break;
        case 5:
            value = kRed - 9;
            break;
        case 4:
            value = kRed - 7;
            break;
        case 3:
            value = kRed - 4;
            break;
        case 2:
            value = kRed - 3;
            break;
        case 1:
            value = kRed + 1;
            break;
        case 0:
            value = kRed + 2;
            break;
        default:
            break;
        }
        return value;
    }

    void Renderer::_renderEField(){
        if(_rf.time_slices.size() > 0){
            double x1,y1,x2,y2;

            for(int i = 0; i < _rf.time_slices.at(0).field.size() ; i++){
                auto point = _rf.time_slices.at(0).field.at(i);

                x1 = 0.5 + point.position.X()/3;
                y1 = 0.5 + point.position.Y()/3;
                x2 = x1 + point.field.X()/30;
                y2 = y1 + point.field.Y()/30;
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
        if( _magnets.positive_positions.size() <= 0 ){
            return;
        }

        for ( int i = 0; i < _magnets.positive_positions.size(); i++ ){
            TEllipse* point = new TEllipse( 0.5 + _magnets.positive_positions[i].first/3,
                                            0.5 + _magnets.positive_positions[i].second/3,
                                            0.008, 0.008);
            point->Draw();
            point->SetFillColor(kBlue -9);
            point->SetLineColor(kBlue -9);
            posBField.push_back(point);
        }

        std::cout << "There are " << _magnets.positive_positions.size() << " positive positions" << std::endl;

        // STATIC FOR NOW SO CLEAR THE LOG
        _magnets.positive_positions.clear();
    }

    void Renderer::Render(TRootEmbeddedCanvas *c){
        canvas = c;
        canvas->GetCanvas()->Clear();
        canvas->DrawBorder();

        // setup the electrons
        _renderElectrons();

        // setup E field
        _renderEField();

        // setup B field
        _renderBField();

        canvas->GetCanvas()->Modified();
        canvas->GetCanvas()->Update();

        render_ready = true;
        //gSystem->Unlink("gui_output.gif");
    }
#pragma endregion RENDER_FILLED


    void Renderer::_updateElectrons(int log_index){
        for(int j = 0; j < electrons.size(); j++){
            electrons.at(j)->SetX1(0.5 + _electrons_log[j].time_slices.at(log_index).position.X()/3);
            electrons.at(j)->SetY1(0.5 + _electrons_log[j].time_slices.at(log_index).position.Y()/3);
            electrons.at(j)->Draw();
        }
    }

    void Renderer::_updateEField(int log_index){
        if (log_index < _rf.time_slices.size()){

            for(int j = 0; j < _rf.time_slices.at(log_index).field.size() ; j++){
                auto point = _rf.time_slices.at(log_index).field.at(j);

                if( _rf.time_slices.at(log_index).field.at(j).magnitude == 0 )
                    continue;

                double x1,y1,x2,y2;
                x1 = 0.5 + point.position.X()/3;
                y1 = 0.5 + point.position.Y()/3;
                x2 = x1 + point.field.X()/30;
                y2 = y1 + point.field.Y()/30;
                rfFieldArrows[j]->SetLineColor(EGradient(point.magnitude));

                rfFieldArrows[j]->DrawArrow(x1, y1, x2, y2, 0.005);
            }

        }
    }

    void Renderer::_updateBField(int log_index){
        // STATIC FOR NOW!!! 
        for(int i = 0; i < posBField.size(); i++){
            posBField[i]->Draw();
        }
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
        static auto start = std::chrono::high_resolution_clock::now();

        // Check if the iteration is over
        if ( i >= _electrons_log[0].time_slices.size()){
            i = 0;
            timer->TurnOff();
            auto end = std::chrono::high_resolution_clock::now();

            auto dur = std::chrono::duration_cast<std::chrono::seconds>(end - start);
            std::cout << "Play took " << dur.count() << " s" << std::endl;
            if (_save_gif){
                _save_gif = false;
                gSystem->Exec("convert -delay 10 -loop 0 temp/gif_frames/*.png animation.gif");
                std::string rmdir_cmd = "rm -r ";
                rmdir_cmd += _temp_gif_frames_path;
                gSystem->Exec(rmdir_cmd.c_str());
            }
        }
        
        canvas->GetCanvas()->Clear();

        _updateEField(i);
        _updateBField(i);
        _updateElectrons(i);

        canvas->GetCanvas()->Modified();
        canvas->GetCanvas()->Update();

        if( _save_gif && i % 3 == 0 ){
            int index = i / 3;
            std::string filename = _temp_gif_frames_path;
            filename += "/gui_output";
            filename += std::to_string(index/1000 % 10);
            filename += std::to_string(index/100 % 10);
            filename += std::to_string(index/10 % 10);
            filename += std::to_string(index % 10);
            filename += ".png";
            canvas->GetCanvas()->Print(filename.c_str());
        }
        
        i++;
    }

    void Renderer::clear(){
        _electrons_log.clear();
        _rf.time_slices.clear();
        _magnets.positive_positions.clear();
    }

    int Renderer::_indexFromTime(float time){

        for(int i = 0; i < _electrons_log[0].time_slices.size(); i++){
            if ( _electrons_log[0].time_slices[i].time == time ){
                return i;
            }
        }

        std::cerr << "No such time." << std::endl;
        return -1;
    }

    void Renderer::GoToTime(float time){
        int slice_index = _indexFromTime(time);

        if ( slice_index == -1)
            return;

        canvas->GetCanvas()->Clear();

        _updateEField(slice_index);
        _updateBField(slice_index);
        _updateElectrons(slice_index);

        canvas->GetCanvas()->Modified();
        canvas->GetCanvas()->Update();
        
    }
}