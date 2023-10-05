#include "gui_sim_dataprovider.h"
#include "../../SIM/basic/vector.h"
#include <fstream>



std::istream& operator>>(std::istream& stream, ElectronSnapshot& e_snapshot){
        std::string line;
        std::getline(stream, line);

        float x,y,z, vx, vy, vz;

        sscanf(line.c_str(), "%f,%f,( %f ; %f ; %f ),( %f ; %f ; %f )", 
                    &e_snapshot.time, &e_snapshot.energy, &x, &y, &z, &vx, &vy, &vz);

        e_snapshot.position = vector3d(x, y, z);
        e_snapshot.velocity = vector3d(vx, vy, vz);

        return stream;
    }

    std::istream& operator>>(std::istream& stream, RFPoint& rf_point){
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

    std::ostream& operator<<(std::ostream& stream, RFPoint& rf_point){
        stream << rf_point.position << "," << rf_point.field << "," << rf_point.magnitude;
        return stream;
    }

    std::istream& operator>>(std::istream& stream, RFSnapshot& rf_snapshot){
        RFPoint point;

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
        //int count = 0;
        while (!stream.eof()) {
            prevpos = stream.tellg();

            std::getline(stream, time_str, ',');

            if ( time_str.size() ){
                time = std::stof(time_str);
            }
            else{
                std::getline(stream, time_str);
                return stream;
            }

            if( stream.fail() ) {
                #ifdef DEBUG
                std::cerr << "Error: " << strerror(errno) << " time_Str : " << time_str << std::endl;
                #endif
                return stream;
            }

            if( time != rf_snapshot.time ){
                stream.seekg(prevpos);
                return stream;
            }

            stream >> point;

            rf_snapshot.field.push_back(point);
            if(point.magnitude > rf_snapshot.max){
                rf_snapshot.max = point.magnitude;
            }
            //count++;
        }

        return stream;
    }

//namespace RhodotronSimulatorGUI::data{

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
                e.b_index = i + 1;
                e.e_index = j + 1;

                stream.open(filename, std::ios::in);
                if(stream.fail()) { continue; }

                std::string firstline;
                std::getline(stream, firstline);
                float time_step = 0, prev_time = 0;

                while (!stream.eof()) {
                    ElectronSnapshot snapshot;
                    stream >> snapshot;

                    if(snapshot.energy > _Emax){
                        _Emax = snapshot.energy;
                    }

                    e.time_slices.push_back(snapshot);

                    if(time_step != 0 && time_step - snapshot.time + prev_time > 0.0001){
                        #ifdef DEBUG
                        std::cerr << "   Inconsistent time step in electron log : " << filename << " t:" << snapshot.time << std::endl;
                        #endif
                    }

                    time_step = snapshot.time - prev_time;
                    prev_time = snapshot.time;
                }
                _electrons_log.push_back(e);
                stream.close();

                _electron_time_step = time_step;
            }
        }
    }

    void DataProvider::_fillEField(){
        std::ifstream stream;
        stream.open(_rflog_path, std::ios::in);

        if( stream.fail()) { return; }
        std::string firstline;
        std::getline(stream, firstline);

        float time_step = 0, prev_time = 0;

        while (!stream.eof()) {
            RFSnapshot snapshot;
            stream >> snapshot;

            if ( stream.fail()){
                #ifdef DEBUG
                std::cerr << "stream failed at Renderer::_fillEField() : " << snapshot.time << " err = \"" << strerror(errno)  << "\" ; Checking for severeness." << std::endl;
                #endif
                if ( _rf.time_slices.size() && _rf.time_slices.back().field.size() != snapshot.field.size()){
                    #ifdef DEBUG
                    std::cerr << "  Fail severity is high, ignoring time : " << snapshot.time << std::endl;
                    #endif
                    continue; 
                }
            }
                
            
            _rf.time_slices.push_back(snapshot);
            if(snapshot.max > _rf.RF_max){
                _rf.RF_max = snapshot.max;
            }
            

            if(time_step != 0 && time_step - snapshot.time + prev_time > 0.0001){
                #ifdef DEBUG
                std::cerr << "   Inconsistent time step in rf log, t:" << snapshot.time << " time_step:" << time_step << std::endl;
                #endif
            }

            time_step = snapshot.time - prev_time;
            prev_time = snapshot.time;
        }
        stream.close();
        _efield_time_step = time_step;
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

    void DataProvider::clearLogs(){
        _electrons_log.clear();
        _rf.time_slices.clear();
        _rf.RF_max = 0;
        _magnets.non_zero_positions.clear();
        _data_filled = false;
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


    const float DataProvider::GetElectronTimeStep(){
        return _electron_time_step;
    }

    const float DataProvider::GetEFieldTimeStep(){
        return _efield_time_step;
    }

//}