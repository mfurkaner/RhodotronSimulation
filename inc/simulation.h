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

class Simulator{        // E in MV/m,   En in MeV,   B in T,    t in
private:
    MultiThreadEngine MTEngine;
    RFField E_field;
    MagneticField B_field;
    double simulation_time = 0;
    double end_time = 45;
    double phase_lag;
    Bunch2D bunch;
    double time_interval = dT;
    uint64_t total_steps = 0;
    double gun_active_time = 1; // ns
    uint64_t num_of_electrons = 1;
    unsigned int thread_count = 1;
    bool multi_threading = false;
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

    void setEmax(double E_max){E_field.setEmax(E_max);}
    void setEin(double E_in){ bunch.setEin(E_in);}
    void setNumberofElectrons(uint64_t num_of_electrons){
        double E_in = bunch.getEin();
        this->num_of_electrons = num_of_electrons;
        bunch = Bunch2D(num_of_electrons);
        bunch.setEin(E_in);
    }
    void addMagnet(double B, double r, vector3d position);
    void addMagnet(Magnet m);
    void setPhaseLag(double phase_lag){ this->phase_lag = phase_lag;}
    void setEndTime(double end_time){ this->end_time = end_time;}

    void run(DataStorage& ds, DataStorage& rf);
    double getAverageEnergy();
    std::vector<double> getRelativeEnterDistance(){return B_field.getRelativeEnterDistance();}
    Electron2D& getElectronWithMaxEnergy();
};