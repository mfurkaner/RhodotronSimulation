#ifndef VECTOR_H
    #include "vector.h"
#endif
#ifndef CONSTS_H
    #include "consts.h"
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
    vector3d actOn(Electron2D& e);
    void update(double time);
};

class Magnet{
    public:
    double r;
    vector3d position;
    double B;
    Magnet(){};
    Magnet(double B, double r, vector3d position) : B(B), r(r), position(position){}
};

class MagneticField{
    std::vector<Magnet> magnets;
    int isInside(vector3d position);
public:
    void addMagnet(double B, double r, vector3d position);
    vector3d getField(vector3d position);
    vector3d actOn(Electron2D& e);
};

bool isInsideHalfSphere(vector3d e_position, double r, vector3d hs_position);