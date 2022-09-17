#ifndef CONSTS_H
    #include "consts.h"
#endif

#ifndef VECTOR_H
    #include "vector.h"
#endif

#ifndef FIELDS_H
    #include "fields.h"
#endif
#ifndef GNUPLOT_H
    #include "gnuplot.h"
#endif


using namespace std;
#define RHODO2D_H
//#define RUNGE_KUTTA
#define LEAP_FROG

double Eradial(double r, double time, double phase);


typedef pair<double, double> giris_cikis_tpair;

struct ElectronLog{
    double time;
    vector3d pos;
    vector3d vel;
    double energy;
};


std::ostream& operator<<(std::ostream& stream, ElectronLog& log);
class Electron2D{
    vector<ElectronLog> log;
public:
    uint64_t index;
    double Et = E0;
    vector3d pos;
    vector3d vel;
    vector< double > enerjiler;
    vector< giris_cikis_tpair > t_giris_cikis;
    bool isinside = true;
    Electron2D(){}
    Electron2D(double Ein, vector3d position, vector3d direction){
        Et += Ein;
        t_giris_cikis.push_back(giris_cikis_tpair(0,0));
        pos = position;
        vel = direction*get_vel();
    }
    
    double get_vel();
    void setEin(double E_in){this->Et = E_in + E0; vel.setX(get_vel());};
    void print_electron_info();
    void move(double dt);
    void move(vector3d acc, double dt);
    void move(vector3d acc, vector3d jerk, double dt);
    void accelerate(vector3d acc, double dt);
    void accelerate(vector3d acc, vector3d jerk, double dt);

    void saveInfo(double t){
        ElectronLog info;
        info.time = t;
        info.pos = pos;
        info.vel = vel;
        info.energy = Et - E0;
        log.push_back(info);
    }
    void loge(DataStorage& path){
        for(int i = 0; i < log.size() ; i++){
            path << log[i] << "\n";
        }
    }

    vector<ElectronLog>& getLog(){
        return log;
    }

    double beta(){
        return vel.magnitude()/c;
    }
    double gamma(){
        return 1/(sqrt(1 - beta()*beta()));
    }
};


typedef pair<double, double> max_energy_rms_pair;

class Bunch2D{
private:
    int e_count = 1;
    double initial_length_ns = 1;
    double ns_between = 0.1;
    int index_fastest = 0;
    double max_energy = 0;
    double entry_time = 0;
    double E_in = 0.04;
    vector3d gun_pos;
    vector3d gun_dir;
public:
    vector<Bunch2D> subBunchs;
    vector<Electron2D> e;
    Bunch2D(unsigned int num_of_electrons, double Ein, vector3d gunpos, vector3d gundir, double gun_ns){
        E_in = Ein;
        e_count = num_of_electrons;
        initial_length_ns = gun_ns;
        gun_pos = gunpos;
        gun_dir = gundir;
        for(int i = 0 ; i < num_of_electrons ; i++){
            e.push_back(Electron2D(Ein, gunpos, gundir));
        }
        ns_between = initial_length_ns/(e_count - 1);
    }

    Bunch2D(){
        Bunch2D(1, 0.4, vector3d(-0.753,0,0), vector3d(1,0,0), 0);
    }

    void saveInfo(double time){
        for(Electron2D& i : e){
            i.saveInfo(time);
        }
    }

    Electron2D& getFastest();

    void setEntryTime(double entry_time){this->entry_time = entry_time;}
    void setEin(double E_in){ this->E_in = E_in ; for (int i = 0; i < e.size() ; i++){ e[i].setEin(E_in);}}
    void setNSLen(double len){ initial_length_ns = len; ns_between = initial_length_ns/(e_count - 1);}
    double getEin(){ return E_in;}
    void interact(RFField& E, MagneticField& B, double time, double time_interval);
    void divide(unsigned int num);
    void concat();
    Bunch2D& subBunch(unsigned int index);
    vector<Bunch2D*> subBunchPtr();
    double E_ave();
    double E_rms();
    void reset();
    void print_bunch_info();
    void print_summary();

    void logPaths(vector<DataStorage>& pathStorage){
        for(int i = 0; i < e.size() && i < pathStorage.size(); i++){
            pathStorage[i].open();
            e[i].loge(pathStorage[i]);
            pathStorage[i].close();
        }
    }
};


class Gun{
    double Ein;
    double gun_period;
    double gun_active_time;
    vector3d gunpos;
    vector3d gundir = vector3d(1,0,0);
    uint64_t bunch_count = 0;
    uint64_t e_per_bunch = 1;
    void setNSLen(double len){ for (int i = 0; i < bunchs.size() ; i++){  bunchs[i].setNSLen(len); }}
public:
    vector<Bunch2D> bunchs;
    Gun(){}
    Gun(double Ein, double gun_active_time, double pulse_interval, vector3d gunpos){
        this->Ein = Ein;
        this->gun_active_time = gun_active_time;
        gun_period = pulse_interval;
        this->gunpos = gunpos;
    }
    void interact(RFField& E, MagneticField& B, double time, double time_interval){
        int interacters = 1 + time/gun_period;
        for(int i = 0; i < interacters && i < bunchs.size(); i++){
            bunchs.at(i).interact(E, B, time, time_interval);
        }
    }
    void addBunch(unsigned int num_of_electrons, double Ein){
        e_per_bunch = num_of_electrons;
        bunchs.push_back(Bunch2D(num_of_electrons, Ein, gunpos, gundir, gun_active_time));
        bunch_count = bunchs.size();
        bunchs[bunch_count - 1].setEntryTime( (bunch_count - 1)*gun_period );
    }
    void setGunActiveTime(double gt){gun_active_time = gt; setNSLen(gt);}
    void setGunInterval(double guninterval){gun_period = guninterval;}
    void setGunPos(vector3d gun_pos){gunpos = gun_pos;}
    void setGunDir(vector3d gun_dir){gundir = gun_dir;}
    void setEin(double Ein) {this->Ein = Ein ; for (int i = 0; i < bunchs.size() ; i++){ bunchs[i].setEin(Ein);}; }
    void setNumberOfElectrons(uint64_t e_num){ 
        e_per_bunch = e_num;
        uint64_t bnum = bunch_count;
        while( !bunchs.empty() ){
            bunchs.erase(bunchs.begin());
        }
        bunch_count = bunchs.size();
        for(int i = 0; i < bnum; i++){
            addBunch(e_num, Ein);
        }
    }
    void setNumberOfBunchs(uint64_t b_num){
        while( !bunchs.empty() ){
            bunchs.erase(bunchs.begin());
        }
        bunch_count = bunchs.size();
        for(int i = 0; i < b_num; i++){
            addBunch(e_per_bunch, Ein);
        }
    }
    
    void saveInfo(double time){
        for(int i = 0; i< bunchs.size(); i++){
            bunchs[i].saveInfo(time);
        }
    }

    void logPaths(vector<vector<DataStorage>>& pathsStorage, std::string pathsPath){
        std::string rmcmd = "rm " + pathsPath + "**";
        system(rmcmd.c_str());
        for(int i = 0 ; i < bunchs.size() ; i++){
            bunchs[i].logPaths(pathsStorage.at(i));
        }
    }

};