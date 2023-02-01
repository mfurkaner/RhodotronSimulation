#include "../particles/gun.h"
#include "../fields/fields.h"
#include "../config/configuration.h"
#include "../gnuplot/gnuplot.h"
#include "../mt/multithreadengine.h"

#include <string>
#include <memory>

#define SIMULATION_H

class Simulator{        // E in MV/m,   En in MeV,   B in T,    t in
protected:
    Gun gun;
    double Emax; double freq = 107.5; double phase_lag = 0; 
    DataStorage EfieldStorage = DataStorage("xy/rf.txt");
    DataStorage BfieldStorage = DataStorage("xy/magnet.txt");
    vector<vector<DataStorage> > pathsStorage;
    std::string configPath = "xy/settings.txt";
    std::string pathsPath = "xy/paths/";

    std::string EfieldLogHeader = "# time(ns),( px(m) , py(m) , pz(m) ),( Ex , Ey , Ez ),|E|";
    std::string BfieldLogHeader = "# posx posy";
    std::string eLogHeader = "# time(ns),E(MeV),( px , py , pz ),( vx , vy , vz )";

    double simulation_time = 0;
    double dummy_time = 0;
    double start_time = 0;
    double end_time = 45;   

    double time_interval = 0.0001;
    uint64_t STEPS_TAKEN = 0;
    double GUN_ACTIVE_TIME = 1;         // ns
    double GUN_PERIOD = 9.3;
    uint64_t NUM_OF_ELECTRONS = 1;
    uint64_t NUM_OF_BUNCHS = 1;
    double Ein = 0.04;

    vector3d gunPosition;
    vector3d gunDirection = vector3d(1,0,0);

    shared_ptr<mutex> ui_mutex;
    MultiThreadEngine MTEngine;
    unsigned int MAX_THREAD_COUNT = 1;
    bool MULTI_THREAD = false;
    bool DEBUG = false;

public:
    Simulator():ui_mutex(make_shared<mutex>()){}
    ~Simulator(){}

    void enableMultiThreading(unsigned int thread_count){
        if ( thread_count == 1) return;
        this->MAX_THREAD_COUNT = thread_count;
        MULTI_THREAD = true;
        MTEngine.setThreadCount(thread_count);
        gun.enableMT(thread_count);
    }

    void setdT(double dT){time_interval = dT;}
    void setEin(double E_in){ gun.setEin(E_in); Ein = E_in;}
    void setNumberofElectrons(uint64_t num_of_electrons){
        this->NUM_OF_ELECTRONS = num_of_electrons;
        gun.setNumberOfElectrons(NUM_OF_ELECTRONS);
    }
    void setNumberofBunchs(uint64_t num_of_bunchs){
        NUM_OF_BUNCHS = num_of_bunchs;
        gun.setNumberOfBunchs(NUM_OF_BUNCHS);
    }
    void setStartTime(double starttime){ simulation_time += starttime - start_time; start_time = starttime;}
    void setEndTime(double end_time){ this->end_time = end_time;}
    void setGunActiveTime(double gun_ns){
        GUN_ACTIVE_TIME = gun_ns;
        gun.setGunActiveTime(gun_ns);
    }
    void setGunPeriod(double pi){
        GUN_PERIOD = pi;
        gun.setGunInterval(pi);
    }
    void setGunPosition(vector3d pos){
        gunPosition = pos;
        setNumberofElectrons(NUM_OF_ELECTRONS);
    }
    void setGunDirection(vector3d dir){
        gunDirection = dir;
        setNumberofElectrons(NUM_OF_ELECTRONS);
    }

    void setRFPath(std::string path){EfieldStorage.filepath = path;}
    void setBPath(std::string path){BfieldStorage.filepath = path;}
    void setPathsPath(std::string path){pathsPath = path;}
    void setConfigPath(std::string path){configPath = path;}

    virtual void run();
    void saveElectronsInfo(double time);
    void openLogs(){
        for(int i = 0; i < NUM_OF_BUNCHS; i++){
            vector<DataStorage> ds;
            for(int j = 0; j < NUM_OF_ELECTRONS ; j++){
                string path = pathsPath + "b" + to_string(i + 1) + "_e" + to_string( j + 1) + ".dat";
                ds.push_back(DataStorage(path));
            }
            pathsStorage.push_back(ds);
        }
        EfieldStorage.open();
        BfieldStorage.open();

        EfieldStorage << EfieldLogHeader << "\n";
        BfieldStorage << BfieldLogHeader << "\n";
    }
    void closeLogs(){
        EfieldStorage.close();
        BfieldStorage.close();
    }

    const unsigned int log_interval(){
        return 0.1/time_interval;
    }

    //double getAverageEnergy();
    //std::vector<double> getRelativeEnterDistance(){return B_field.getRelativeEnterDistance();}

    virtual void getConfig(Configuration& config) = 0;
    virtual void logEfield(double time, bool) = 0;
    virtual void logBfield() = 0;
    void logPaths();
    double* getTimePtr(){return &dummy_time;}
    shared_ptr<mutex> getUIMutex() {return ui_mutex;}
};


class RhodotronSimulator : public Simulator{
private:
    double R1;
    double R2;
    CoaxialRFField E_field;
    MagneticField B_field;

    void _runMT();
    void _runST();
public:
    RhodotronSimulator(double phase_lag) : Simulator(){
        this->phase_lag = phase_lag;
        E_field = CoaxialRFField(phase_lag);
        E_field.setR1(R1);
        E_field.setR2(R2);
        E_field.setEmaxPos(R1);
        setGunDirection(vector3d(1,0,0));
        setGunPosition(vector3d(-R2,0,0));
    }
    RhodotronSimulator(Configuration& config) : Simulator(){
        E_field = CoaxialRFField(phase_lag);
        getConfig(config);
        setGunDirection(vector3d(1,0,0));
    }


    void setFreq(double frequency){ 
        freq = frequency;
        E_field.setFreq(freq);
    }
    void setPhaseLag(double phase_lag){ 
        this->phase_lag = phase_lag;
        E_field.setPhaseLag(phase_lag);
    }
    void setEmax(double E_max){E_field.setEmax(E_max); Emax = E_max;}
    void setR1(double r1){
        R1 = r1;
        E_field.setR1(R1);
    }
    void setR2(double r2){
        R2 = r2;
        E_field.setR2(R2);
        setGunPosition(vector3d(-R2,0,0));
    }
    void addMagnet(double B, double r, vector3d position);
    void addMagnet(Magnet m);
    
    void updateSimulation();
    void getConfig(Configuration& config);
    void logEfield(double time, bool end){
        E_field.log(EfieldStorage, time, end);
    }
    void logBfield(){
        B_field.log(BfieldStorage);
    }

    void run();
    void notifyUI(double time);
};
