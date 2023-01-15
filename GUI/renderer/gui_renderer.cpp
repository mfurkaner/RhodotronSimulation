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

    void Renderer::_fillElectrons(){

        _enum = 40;
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

    void Renderer::_fillRf(){
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

    void Renderer::_fillMagnets(){
        std::ifstream stream;
        stream.open(_mlog_path, std::ios::in);

        if( stream.fail()) { return; }
        std::string filename;
        std::getline(stream, filename);

        while (!stream.eof()) {
            std::pair<float, float> position;
            float x, y;
            stream >> x >> "," >> y >> "\n";

            position.first = x;
            position.second = y;
            
            if ( stream.fail()) { return; }

            _magnets.positive_positions.push_back(position);
        }
        stream.close();
    }

    void Renderer::fillLogs(){
        auto fill_start = std::chrono::high_resolution_clock::now();
        _fillElectrons();
        _fillRf();
        //_fillMagnets();

        auto fill_end = std::chrono::high_resolution_clock::now();

        auto fill_time = std::chrono::duration_cast<std::chrono::milliseconds>(fill_end - fill_start);

        std::cout << "Fill took " << fill_time.count()/1000.0 << " s" << std::endl;
    }

    void Renderer::render(TRootEmbeddedCanvas *c){
        canvas = c;
        canvas->GetCanvas()->Clear();
        canvas->DrawBorder();

        // setup the electrons
        std::cout << "log size : " << _electrons_log.size() << std::endl;
        for(int i = 0; i < _electrons_log.size(); i++){
            TEllipse* point = new TEllipse( 0.5 + _electrons_log[i].time_slices.at(0).position.X()/3,
                                            0.5 + _electrons_log[i].time_slices.at(0).position.Y()/3,
                                            0.008, 0.008);
            point->Draw();
            point->SetFillColor(5);
            //point->SetLineColor(5);
            es.push_back(point);
        }

        // setup E field
        if (_rf.time_slices.size() > 0){
            for(int i = 0; i < _rf.time_slices.at(0).field.size() ; i++){
                auto point = _rf.time_slices.at(0).field.at(i);

                double x1,y1,x2,y2;
                x1 = 0.5 + point.position.X()/3;
                y1 = 0.5 + point.position.Y()/3;
                x2 = x1 + point.field.X()/30;
                y2 = y1 + point.field.Y()/30;

                TArrow* rfArrow = new TArrow(x1, y1, x2, y2, 0.005);

                if( point.magnitude != 0.0 ){
                    rfArrow->Draw();
                }

                rfFieldArrows.push_back(rfArrow);
            }
        }
        canvas->GetCanvas()->Modified();
        canvas->GetCanvas()->Update();

        render_ready = true;
        gSystem->Unlink("gui_output.gif");
        //run_rendered();
    }

    void Renderer::run_rendered(){
        if(render_ready == false || timer->IsRunning()) return;

        timer->Connect("Timeout()", "RhodotronSimulatorGUI::renderer::Renderer", this, "iterate()");			
        timer->TurnOn();
    }

    void Renderer::iterate(){
        static int i = 0;
        if ( i >= _electrons_log[0].time_slices.size()){
            i = 0;
            timer->TurnOff();
            canvas->GetCanvas()->Print("gui_output.gif++");
        }
        canvas->GetCanvas()->GetPad(0)->cd();
        canvas->GetCanvas()->Clear();

        auto update_start = std::chrono::high_resolution_clock::now();

        // update the electrons
        for(int j = 0; j < es.size(); j++){
            es.at(j)->SetX1(0.5 + _electrons_log[j].time_slices.at(i).position.X()/3);
            es.at(j)->SetY1(0.5 + _electrons_log[j].time_slices.at(i).position.Y()/3);
            es.at(j)->Draw();

        }
        //auto e_end = std::chrono::high_resolution_clock::now();
        //auto e_dur = std::chrono::duration_cast<std::chrono::milliseconds>(e_end - update_start);

        //std::cout << "Electron update took " << e_dur.count() << " ms" << std::endl;

        // update the rf field
        if (i < _rf.time_slices.size()){
            for(int j = 0; j < _rf.time_slices.at(i).field.size() ; j++){
                auto point = _rf.time_slices.at(i).field.at(j);


                if( _rf.time_slices.at(i).field.at(j).magnitude == 0 )
                    continue;

                double x1,y1,x2,y2;
                x1 = 0.5 + point.position.X()/3;
                y1 = 0.5 + point.position.Y()/3;
                x2 = x1 + point.field.X()/30;
                y2 = y1 + point.field.Y()/30;

                rfFieldArrows[j]->DrawArrow(x1, y1, x2, y2, 0.005);
            }
        }

        canvas->GetCanvas()->Modified();
        canvas->GetCanvas()->Update();
        if( i % 3 == 0){
            canvas->GetCanvas()->Print("gui_output.gif+");
        }
        
        i++;
    }

    void Renderer::clear(){
        _electrons_log.clear();
        _rf.time_slices.clear();
        _magnets.positive_positions.clear();
    }


    void Renderer::GoToTime(float time){
        //std::cout <<"Requested time : "<<time << " first time is : " << _electrons_log[0].time_slices[0].time <<std::endl;
        int slice_index = -1;

        for(int i = 0; i < _electrons_log[0].time_slices.size(); i++){
            if ( _electrons_log[0].time_slices[i].time == time ){
                slice_index = i;
                break;
            }
        }

        if ( slice_index == -1){
            std::cerr << "No such time." << std::endl;
            return;
        }

        canvas->GetCanvas()->Clear();

        for(int j = 0; j < es.size(); j++){
            es.at(j)->SetX1(0.5 + _electrons_log[j].time_slices.at(slice_index).position.X()/3);
            es.at(j)->SetY1(0.5 + _electrons_log[j].time_slices.at(slice_index).position.Y()/3);
            es.at(j)->Draw();
        }
        

        if (slice_index < _rf.time_slices.size()){
            for(int j = 0; j < _rf.time_slices.at(slice_index).field.size() ; j++){
                auto point = _rf.time_slices.at(slice_index).field.at(j);


                if( _rf.time_slices.at(slice_index).field.at(j).magnitude == 0 )
                    continue;

                double x1,y1,x2,y2;
                x1 = 0.5 + point.position.X()/3;
                y1 = 0.5 + point.position.Y()/3;
                x2 = x1 + point.field.X()/30;
                y2 = y1 + point.field.Y()/30;

                rfFieldArrows[j]->DrawArrow(x1, y1, x2, y2, 0.005);
            }
        }

        canvas->GetCanvas()->Modified();
        canvas->GetCanvas()->Update();
        
    }
}