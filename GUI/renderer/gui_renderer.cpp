#include "gui_renderer.h"
#include "../../SIM/basic/vector.cpp"
#include <fstream>
#include <tuple>
#include "TPoint.h"
#include "TTimer.h"

    std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::ElectronSnapshot& e_snapshot){
        stream >> e_snapshot.time >> "," >> e_snapshot.energy >> "," >> e_snapshot.position >> "," >> e_snapshot.velocity >> "\n";
        return stream;
    }

    std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::RFPoint& rf_point){
        stream >> rf_point.position >> "," >> rf_point.field >> "," >> rf_point.magnitude;
        return stream;
    }

    std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::renderer::RFSnapshot& rf_snapshot){
        RhodotronSimulatorGUI::renderer::RFPoint point;
        stream >> rf_snapshot.time >> "," >> point >> "\n";

        if ( stream.fail() ) { 
            return stream;
        }

        rf_snapshot.field.push_back(point);

        float time = rf_snapshot.time;
        std::streampos prevpos;

        while (!stream.eof()) {
            prevpos = stream.tellg();

            stream >> time >> "," >> point >> "\n";
            if( stream.fail() ) {return stream;}

            if( time != rf_snapshot.time ){
                stream.seekg(prevpos);
                return stream;
            }
            rf_snapshot.field.push_back(point);
        }

        return stream;
    }

namespace RhodotronSimulatorGUI::renderer{

    void Renderer::_fillElectrons(){
        for (uint32_t i = 0; i < _enum ; i++){

            ElectronLog e;
            std::ifstream stream;
            std::string filename = _elog_path + "e" + std::to_string(i + 1) + ".txt";

            std::cout << "reading from " << filename << std::endl;

            stream.open(filename, std::ios::in);

            if(stream.fail()) { continue; }

            std::getline(stream, filename);

            while (!stream.eof()) {
                ElectronSnapshot snapshot;
                stream >> snapshot;
                e.time_slices.push_back(snapshot);

                std::cout << "read " << e.time_slices.size() << " e snapshots" << std::endl;
                std::getline(stream, filename);
            }
            _electrons_log.push_back(e);
            stream.close();

            std::cout << "end of this e" << std::endl;

        }
    }

    void Renderer::_fillRf(){
        std::ifstream stream;
        stream.open(_rflog_path, std::ios::in);

        if( stream.fail()) { return; }
        std::string filename;
        std::getline(stream, filename);

        while (!stream.eof()) {
            RFSnapshot snapshot;
            stream >> snapshot;
            
            if ( stream.fail()) { return; }

            _rf.time_slices.push_back(snapshot);
        }

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
        _fillElectrons();
        //_fillRf();
        //_fillMagnets();
    }

    void Renderer::render(TRootEmbeddedCanvas *c){
        canvas = c;
        // setup the electrons
        for(int i = 0; i < _electrons_log.size(); i++){
            TPoint* point = new TPoint( _electrons_log[i].time_slices[0].position.X(), _electrons_log[i].time_slices[0].position.Y() );

            es.push_back(point);
        }

        TTimer *timer = new TTimer(10);		
        timer->Connect("Timeout()", "RhodotronSimulatorGUI::renderer::Renderer", this, "iterate()");			
        timer->TurnOn();

    }

    void Renderer::iterate(){
        static int i = 0;
        if ( i >= _electrons_log[0].time_slices.size()){
            i = 0;
        }
        for(int j = 0; j < es.size(); i++){
            es.at(j)->SetX(_electrons_log[j].time_slices.at(i).position.X());
            es.at(j)->SetY(_electrons_log[j].time_slices.at(i).position.Y());
        }
        canvas->GetCanvas()->Modified();
        canvas->GetCanvas()->Update();
        i++;
    }

    void Renderer::clear(){
        _electrons_log.clear();
        _rf.time_slices.clear();
        _magnets.positive_positions.clear();
    }

}