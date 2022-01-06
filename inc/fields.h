#ifndef VECTOR_H
    #include "vector.h"
#endif
#ifndef CONSTS_H
    #include "consts.h"
#endif
#ifndef GNUPLOT_H
    #include "gnuplot.h"
#endif

#define FIELDS_H

class Electron2D;

class RFField{
private:
    double E;
    double E_max = Emax;
    double E_max_pos = Emax_pos;
    double frequency = freq;
    double phase_lag = 0;
    double r1 = R1;
    double r2 = R2;
    
    double E_radial(double R);
public:
    RFField() { update(0); };
    RFField(double phase_lag) : phase_lag(phase_lag) { update(0); }

    vector3d getField(vector3d position);
    double getField(double R);
    double getE() {return E;}
    void setEmax(double E_max) {this->E_max = E_max;}
    vector3d actOn(Electron2D& e);
    int log(DataStorage& rf, double time);
    void update(double time);
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
    std::vector<bool> hasEntered;
    std::vector<double> relativeEnterDistance;
public:
    void addMagnet(double B, double r, vector3d position);
    void addMagnet(Magnet m);
    vector3d getField(vector3d position);
    vector3d actOn(Electron2D& e);
    void log(DataStorage& magnet);
    std::vector<double> getRelativeEnterDistance(){return relativeEnterDistance;}
};

bool isInsideHalfSphere(vector3d e_position, double r, vector3d hs_position);

double rho_to_magnetic_field(double rho, double Ek);

double magnetic_field_to_rho(double B, double Ek);