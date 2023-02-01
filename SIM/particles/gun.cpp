#include "gun.h"


    Gun::Gun(){}
    Gun::Gun(double Ein, double gun_active_time, double pulse_interval, vector3d gunpos){
        this->Ein = Ein;
        this->gun_active_time = gun_active_time;
        gun_period = pulse_interval;
        this->gunpos = gunpos;
    }

    void Gun::interact(RFField& E, MagneticField& B, double time_interval){
        for(int i = 0; i < bunchs.size(); i++){
            bunchs[i].interact(E, B, time_interval);
        }
    }

    void Gun::addBunch(unsigned int num_of_electrons, double Ein){
        e_per_bunch = num_of_electrons;
        bunchs.push_back(Bunch2D());
        bunch_count = bunchs.size();
    }
    void Gun::setGunActiveTime(double gt){gun_active_time = gt; setNSLen(gt);}
    void Gun::setGunInterval(double guninterval){gun_period = guninterval;}
    void Gun::setGunPos(vector3d gun_pos){gunpos = gun_pos;}
    void Gun::setGunDir(vector3d gun_dir){gundir = gun_dir;}
    void Gun::setEin(double Ein) {this->Ein = Ein ; for (int i = 0; i < bunchs.size() ; i++){ bunchs[i].setEin(Ein);}; }
    void Gun::setNumberOfElectrons(uint64_t e_num){ 
        e_per_bunch = e_num;
        ns_between_each_electron_fire = gun_active_time/e_per_bunch;
    }
    void Gun::setNumberOfBunchs(uint64_t b_num){
        while( !bunchs.empty() ){
            bunchs.erase(bunchs.begin());
        }

        for(int i = 0; i < b_num; i++){
            addBunch(e_per_bunch, Ein);
            bunch_count = bunchs.size();
        }
    }
    
    void Gun::enableMT(uint32_t thread_count){
        _mt_enabled = true;
        _child_thread_count = thread_count;
        for(int i = 0; i < thread_count; i++){
            thread_bunchs.push_back(make_shared<vector<shared_ptr<Electron2D>>>(vector<shared_ptr<Electron2D>>()));
        }
    }

    void Gun::saveInfo(double time){
        for(int i = 0; i< bunchs.size(); i++){
            bunchs[i].saveInfo(time);
        }
    }

    void Gun::logPaths(vector<vector<DataStorage> >& pathsStorage, std::string pathsPath, std::string header){
        std::string rmcmd = "rm " + pathsPath + "**";
        system(rmcmd.c_str());


        for(int i = 0 ; i < bunchs.size() ; i++){
            bunchs[i].logPaths(pathsStorage.at(i), header);
        }
    }

    
    void Gun::fireIfActive(double time){
        // GUN finished firing
        if ( _fired_bunch == bunch_count )
            return;
        else if ( _fired_bunch > bunch_count ){
            perror("Fired more than bunch_count");
        }
        else if ( _fired_e_in_current_bunch >= e_per_bunch ){
            _fired_e_in_current_bunch = 0;
            _firing = false;
            _fired_bunch++;
        }
        if ( time > (ns_between_each_electron_fire * _fired_e_in_current_bunch) + _fired_bunch*gun_period){
            if (_firing == false ) _firing = true;
            bunchs[_fired_bunch].AddElectron(Ein, gunpos, gundir, 0);
            _fired_e_in_current_bunch++;
        }
    }
/*
    void Gun::fireIfActiveMT(double time){
        // GUN finished firing
        if ( _fired_bunch == bunch_count )
            return;
        else if ( _fired_bunch > bunch_count ){
            perror("Fired more than bunch_count");
        }
        else if ( _fired_e_in_current_bunch >= e_per_bunch ){
            _fired_e_in_current_bunch = 0;
            _firing = false;
            _fired_bunch++;
        }
        if ( time > (ns_between_each_electron_fire * _fired_e_in_current_bunch) + _fired_bunch*gun_period){
            if (_firing == false ) _firing = true;

            auto burrowed_e = bunchs[_fired_bunch].AddElectronGiveAddress(Ein, gunpos, gundir);

            if ( _next_up < thread_bunchs.size() ){
                // TODO : Check if thread_bunchs need mutex
                thread_bunchs[_next_up]->push_back(burrowed_e);
                _next_up = (_next_up + 1)%_child_thread_count;
            }

            _fired_e_in_current_bunch++;
        }
    }*/
/*
    void Gun::fireIfActiveMT(double time, int worker_index){
        // GUN finished firing
        if ( _fired_bunch == bunch_count )
            return;
        else if ( _fired_bunch > bunch_count ){
            perror("Fired more than bunch_count");
        }
        else if ( _fired_e_in_current_bunch >= e_per_bunch ){
            _fired_e_in_current_bunch = 0;
            _firing = false;
            _fired_bunch++;
        }
        if ( time > (ns_between_each_electron_fire * _fired_e_in_current_bunch) + _fired_bunch*gun_period){
            if (_firing == false ) _firing = true;

            auto burrowed_e = bunchs[_fired_bunch].AddElectronGiveAddress(Ein, gunpos, gundir, time);

            if ( _next_up < thread_bunchs.size() ){
                // TODO : Check if thread_bunchs need mutex
                thread_bunchs[worker_index]->push_back(burrowed_e);
                _next_up = (_next_up + 1)%_child_thread_count;
            }

            _fired_e_in_current_bunch++;
        }
    }*/

    void Gun::fireAllWithFireTimesMT(){
        for(_fired_bunch= 0; _fired_bunch < bunch_count; _fired_bunch++){
            for(_fired_e_in_current_bunch= 0; _fired_e_in_current_bunch < e_per_bunch; _fired_e_in_current_bunch++){

                double fire_time = (ns_between_each_electron_fire * _fired_e_in_current_bunch) + _fired_bunch*gun_period;

                auto burrowed_e = bunchs[_fired_bunch].AddElectronGiveAddress(Ein, gunpos, gundir, fire_time);

                int thread_index = (_fired_e_in_current_bunch + _fired_bunch*e_per_bunch)%thread_bunchs.size();

                thread_bunchs[thread_index]->push_back(burrowed_e);
            }
        }
    }
