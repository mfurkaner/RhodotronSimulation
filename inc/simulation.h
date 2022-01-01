#ifndef RHODO2D_H
    #include "rhodo2d.h"
#endif
#ifndef FIELDS_H
    #include "fields.h"
#endif

class Simulator{        // E in MV/m,   En in MeV,   B in T,    t in
private:
    RFField E_field;
    MagneticField B_field;
    double simulation_time = 0;
    double end_time = 20;
    double phase_lag;
    Bunch2D bunch;
    double time_interval = dT;
public:
    Simulator(double phase_lag) : phase_lag(phase_lag){
        E_field = RFField(phase_lag);
        bunch = Bunch2D(phase_lag);
    }

    void addMagnet(double B, double r, vector3d position);
    void setPhaseLag(double phase_lag){ this->phase_lag = phase_lag;}
    void setEndTime(double end_time){ this->end_time = end_time;}

    void run();
    double getAverageEnergy();
    Electron2D& getElectronWithMaxEnergy();
};