
#include "../basic/vector.h"
#include "../basic/consts.h"
#include "../gnuplot/gnuplot.h"

#ifndef FIELDS_H
#define FIELDS_H

class Electron2D;

class RFField{
protected:
    double E;                       // MV/m
    double E_max = 0;               // MV/m
    double frequency = 107.5;       // MHz
    double phase_lag = 0;           // degree
public:
    RFField() { update(0); };
    RFField(double phase_lag) : phase_lag(phase_lag) { update(0); }

    virtual vector3d getField(vector3d position){return vector3d(0,0,0);}              // FIX
    virtual double getField(double R){ return 0;}                         // FIX
    virtual int log(DataStorage& rf, double time){return 0;}             // FIX
    vector3d actOn(Electron2D& e);    
    vector3d actOnAndGetRungeKuttaCoef(Electron2D& e, double dt);                  
    
    double getE() {return E;}
    void setEmax(double E_max) {this->E_max = E_max; update(0);}
    void setFreq(double freq){ frequency = freq;}
    void setPhaseLag(double phaselag){ phase_lag = phaselag; update(0);}
    void update(double time);
};

class CoaxialRFField : public RFField{
private:
    double r1 = 0.188;                         // m
    double r2 = 0.753;                         // m
    double E_max_pos = r1;                     // m

    double E_radial(double R);
public:
    CoaxialRFField() { update(0); };
    CoaxialRFField(double phase_lag){ this->phase_lag = phase_lag; update(0); }
    void setR1(double r1) {this->r1 = r1;}
    void setR2(double r2) {this->r2 = r2;}
    void setEmaxPos(double Emaxpos) {E_max_pos = Emaxpos;}     // m

    vector3d getField(vector3d position);
    double getField(double R);
    vector3d actOn(Electron2D& e);
    vector3d actOnAndGetRungeKuttaCoef(Electron2D& e, double dt);        

    int log(DataStorage& rf, double time);
};

class Magnet{
    public:
    double r;
    vector3d position;
    double B;
    Magnet(){};
    Magnet(double B, double r, vector3d position) : B(B), r(r), position(position){}
    double getOptimalB(double E, double minB, double maxB, double stepsize);           // use as if the magnet is rotated like the first magnet
};

class MagneticField{
    std::vector<Magnet> magnets;
    int isInside(vector3d position);
    int isInsideDebug();
    std::vector<bool> hasEntered;
    std::vector<double> relativeEnterDistance;
public:
    void addMagnet(double B, double r, vector3d position);
    void addMagnet(Magnet m);
    vector3d getField(vector3d position);
    vector3d actOn(Electron2D& e);
    vector3d actOnAndGetRungeKuttaCoef(Electron2D& e, double dt);        
    vector3d getJerk(vector3d pos, vector3d vel, vector3d acc);
    void log(DataStorage& magnet);
    std::vector<double> getRelativeEnterDistance(){return relativeEnterDistance;}
};

bool isInsideHalfSphere(vector3d e_position, double r, vector3d hs_position);

double rho_to_magnetic_field(double rho, double Ek);

double magnetic_field_to_rho(double B, double Ek);

#endif