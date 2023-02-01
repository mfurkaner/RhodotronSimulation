#include "gui_sim_dataprovider.h"
#include "../../SIM/basic/vector.cpp"
#include <fstream>



std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::data::ElectronSnapshot& e_snapshot){
        std::string line;
        std::getline(stream, line);

        float x,y,z, vx, vy, vz;

        sscanf(line.c_str(), "%f,%f,( %f ; %f ; %f ),( %f ; %f ; %f )", 
                    &e_snapshot.time, &e_snapshot.energy, &x, &y, &z, &vx, &vy, &vz);

        e_snapshot.position = vector3d(x, y, z);
        e_snapshot.velocity = vector3d(vx, vy, vz);

        return stream;
    }

    std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::data::RFPoint& rf_point){
        std::string line;
        std::getline(stream, line);

        float x,y,z,ex,ey,ez,E;

        sscanf(line.c_str(), "( %f ; %f ; %f ),( %f ; %f ; %f ),%f",
                                 &x, &y, &z, &ex, &ey, &ez, &E);

        rf_point.position = vector3d(x,y,z);
        rf_point.field = vector3d(ex,ey,ez);
        rf_point.magnitude = E;

        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, RhodotronSimulatorGUI::data::RFPoint& rf_point){
        stream << rf_point.position << "," << rf_point.field << "," << rf_point.magnitude;
        return stream;
    }

    std::istream& operator>>(std::istream& stream, RhodotronSimulatorGUI::data::RFSnapshot& rf_snapshot){
        RhodotronSimulatorGUI::data::RFPoint point;

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

namespace RhodotronSimulatorGUI::data{

    void DataProvider::fillLogs(){
        _fillElectrons();
        _fillEField();
        _fillBField();
        _data_filled = true;
    }


    void DataProvider::_fillElectrons(){
        for (uint32_t i = 0; i < _bnum ; i++){
            for (uint32_t j = 0; j < _enum ; j++){

                ElectronLog e;
                std::ifstream stream;
                std::string filename = _elog_path + "b" + std::to_string(i + 1) + "_e" + std::to_string(j + 1) + ".dat";

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

    }

    void DataProvider::_fillEField(){
        std::ifstream stream;
        stream.open(_rflog_path, std::ios::in);

        if( stream.fail()) { return; }
        std::string filename;
        std::getline(stream, filename);


        while (!stream.eof()) {
            RFSnapshot snapshot;
            stream >> snapshot;

            if ( stream.fail()){
                std::cerr << "stream failed at Renderer::_fillEField() : " << snapshot.time << " err = \"" << strerror(errno)  << "\" ; Checking for severeness." << std::endl;

                if ( _rf.time_slices.size() && _rf.time_slices.back().field.size() != snapshot.field.size()){
                    std::cerr << "  Fail severity is high, ignoring time : " << snapshot.time << std::endl;
                    continue; 
                }
            }
                
            
            _rf.time_slices.push_back(snapshot);
        }

        stream.close();
    }

    void DataProvider::_fillBField(){
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

                _magnets.non_zero_positions.push_back(position);
            }
            count++;
        }
        stream.close(); 
    }


    const std::vector<ElectronLog>& DataProvider::GetElectrons(){
        return _electrons_log;
    }

    const RFLog& DataProvider::GetEField(){
        return _rf;
    }

    const StaticMagneticFieldLog& DataProvider::GetStaticBField(){
        return _magnets;
    }


}