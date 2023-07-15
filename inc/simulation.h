#ifndef RHODO2D_H
    #include "rhodo2d.h"
#endif
#ifndef FIELDS_H
    #include "fields.h"
#endif
#ifndef GNUPLOT_H
    #include "gnuplot.h"
#endif
#ifndef MTENGINE_H
    #include "multithreadengine.h"
#endif
#include <string>

class Simulator{        // E in MV/m,   En in MeV,   B in T,    t in
private:
    MultiThreadEngine MTEngine;
    RFField E_field;
    StaticConstantEField S_C_E_field;
    MagneticField B_field;
    double simulation_time = 0;
    double end_time = 45;
    double phase_lag;
    Bunch2D bunch;
    double time_interval = dT;
    uint64_t total_steps = 0;
    double gun_active_time = 1; // ns
    uint64_t num_of_electrons = 100;
    unsigned int thread_count = 1;
    bool multi_threading = false;
    DataStorage EfieldStorage = DataStorage("xy/rf.txt");
    DataStorage StatEfieldStorage = DataStorage("xy/statE.txt"); 
    DataStorage BfieldStorage = DataStorage("xy/magnet.txt");
    vector<DataStorage> pathsStorage;

    bool _RF_active = true;
    bool _Static_E_active = false;
    bool _Static_B_active = true;
public:
    Simulator(double phase_lag) : phase_lag(phase_lag){
        E_field = RFField(phase_lag);
        bunch = Bunch2D(num_of_electrons);
    }

    void enableMultiThreading(unsigned int thread_count){
        this->thread_count = thread_count;
        multi_threading = true;
        MTEngine = MultiThreadEngine(thread_count);
    }

    void disableRF(){_RF_active = false;}
    void disableStaticB(){_Static_B_active = false;}
    void disableStaticE(){_Static_E_active = false;}

    void setEmax(double E_max){E_field.setEmax(E_max);}
    void setEin(double E_in){ bunch.setEin(E_in);}
    void setNumberofElectrons(uint64_t num_of_electrons){
        double E_in = bunch.getEin();
        this->num_of_electrons = num_of_electrons;
        bunch = Bunch2D(num_of_electrons);
        bunch.setEin(E_in);
    }
    void setStaticConstantEfield(vector3d E, double x, double y);
    void addMagnet(double B, double r, vector3d position);
    void addMagnet(Magnet m);
    void setPhaseLag(double phase_lag){ this->phase_lag = phase_lag;}
    void setEndTime(double end_time){ this->end_time = end_time;}
    void run();
    void runBonly();
    void runStaticEonly();
    void saveElectronsInfo(double time);
    void openLogs(){

        for(int i = 0; i < num_of_electrons ; i++){
            string path = "xy/paths/e" + to_string(i+1) + ".txt";
            pathsStorage.push_back(DataStorage(path));
        }
        EfieldStorage.open();
        StatEfieldStorage.open();
        BfieldStorage.open();
    }
    void closeLogs(){
        EfieldStorage.close();
        StatEfieldStorage.close();
        BfieldStorage.close();
    }
    void logEfield(double time){
        E_field.log(EfieldStorage, time);
    }
    void logStaticEfield(){
        S_C_E_field.log(StatEfieldStorage);
    }
    void logBfield(){
        B_field.log(BfieldStorage);
    }
    void logPaths();

    unsigned int log_interval(){
        return 0.1/time_interval;
    }

    double getAverageEnergy();
    std::vector<double> getRelativeEnterDistance(){return B_field.getRelativeEnterDistance();}
    Electron2D& getElectronWithMaxEnergy(){
        return bunch.e[0];
    }
};