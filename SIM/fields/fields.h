
#include "../basic/vector.h"
#include "../basic/consts.h"
#include "../basic/datastorage.h"
#include <memory>
#include <map>

#ifndef FIELDS_H
#define FIELDS_H


class RFField{
protected:
    double phase_lag = 0;           // degree
    double frequency = 107.5;       // MHz
    double lambda = c/(107.5 * 1e+6);
public:
    RFField() { update(0);};
    RFField(double phase_lag) : phase_lag(phase_lag) { update(0); }
    virtual ~RFField() {}

    virtual vector3d getEfield(vector3d& position) const {return vector3d(0,0,0);}              // FIX
    double getEfield(double R)const{ return 0;}                         // FIX
    virtual int log(DataStorage& rf, double time, bool end = false){return 0;}             // FIX
    //vector3d actOn(Electron& e);    
    //vector3d actOnAndGetRungeKuttaCoef(Electron& e, double dt);                  
    

    void setFreq(double freq){ frequency = freq;}
    void setPhaseLag(double phaselag){ phase_lag = phaselag; update(0);}
    virtual void update(double time);

};

class CoaxialRFField : public RFField{
private:
    double E;                       // MV/m
    double E_max = 0;               // MV/m

    double r1 = 0.188;                         // m
    double r2 = 0.753;                         // m
    double E_max_pos = r1;                     // m
    std::vector<CoaxialRFField*> _childs;
    double E_radial(double R) const;
public:
    CoaxialRFField() { update(0); /*_childs.reserve(0);*/}
    CoaxialRFField(double phase_lag){ this->phase_lag = phase_lag; update(0); }
    ~CoaxialRFField() override;

    double getE() {return E;}
    void setEmax(double E_max) {this->E_max = E_max; update(0);}
    void setR1(double r1) {this->r1 = r1; this-> E_max_pos = r1;}
    void setR2(double r2) {this->r2 = r2;}
    void setEmaxPos(double Emaxpos) {E_max_pos = Emaxpos;}     // m

    vector3d getEfield(vector3d& position) const override;
    double getEfield(double R) const;
    //vector3d actOn(Electron& e);
    //vector3d actOnAndGetRungeKuttaCoef(Electron& e, double dt);    
    void update(double time) override; 

    int log(DataStorage& rf, double time, bool end = false) override;

    std::shared_ptr<CoaxialRFField> Copy();
    void split(uint32_t amount_of_child);
    CoaxialRFField* child(uint32_t index);
};

struct RFFieldOnConstXYZPoint{
    double x;
    vector3d E;
};

struct RFFieldOnConstZYLine{
    double y;
    std::vector<RFFieldOnConstXYZPoint> field1D;
};

struct RFFieldOnConstZPlane{
    double z;
    std::vector<RFFieldOnConstZYLine> field2D;
};

struct RFFieldSnapshot{
    double E_field_position_step;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    double zmin;
    double zmax;
    vector3d Emax_pos;
    std::vector<RFFieldOnConstZPlane> Efield;
    //std::map<vector3d, vector3d> Bfield;
};

struct RFFieldIndex{
    size_t x;
    size_t y;
    size_t z;
};

struct StaticEMField{
    std::map<vector3d, vector3d> Efield;
    std::map<vector3d, vector3d> Bfield;
};

class ImportedRFField : public RFField{
private:
    RFFieldSnapshot _fieldMaxValue;
    double _mult_time_coeff;

public:
    ImportedRFField(double phaselag):RFField(phaselag){}
    vector3d getEfield(vector3d& position) const override;
    double getEfield(double R) const = delete;

    RFFieldIndex getNearestEPosition(vector3d& position) const;
    vector3d getEmaxPosition();

    void SetEmaxInLine(double theta, double newEmax);
    void Multiply(double mult_const);

    void update(double time) override;

    void Import(std::string filepath);

};

class Magnet{
    public:
    double r;
    vector3d position;
    double B;
    Magnet(){};
    Magnet(double B, double r, vector3d position) : r(r),  position(position), B(B){}
    //double getOptimalB(double E, double minB, double maxB, double stepsize);           // use as if the magnet is rotated like the first magnet
};

class MagneticField{
    std::vector<Magnet> magnets;
    std::vector<bool> hasEntered;
    std::vector<double> relativeEnterDistance;
    std::vector<MagneticField*> _childs;

    int isInsideDebug();
public:
    ~MagneticField();
    int isInside(vector3d position) const;

    void addMagnet(double B, double r, vector3d position);
    void addMagnet(Magnet m);
    vector3d getField(vector3d position) const;
    //vector3d actOn(Electron& e);
    //vector3d actOnAndGetRungeKuttaCoef(Electron& e, double dt);        
    vector3d getJerk(vector3d pos, vector3d vel, vector3d acc);
    void log(DataStorage& magnet);
    std::vector<double> getRelativeEnterDistance(){return relativeEnterDistance;}

    std::shared_ptr<MagneticField> LightWeightCopy();

    void split(uint32_t amount_of_child);
    MagneticField* child(uint32_t index);
};

bool isInsideHalfSphere(vector3d e_position, double r, vector3d hs_position);

double rho_to_magnetic_field(double rho, double Ek);

double magnetic_field_to_rho(double B, double Ek);

#endif