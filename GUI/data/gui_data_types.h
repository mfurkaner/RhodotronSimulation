#ifndef GUI_DATA_TYPES_H
#define GUI_DATA_TYPES_H

#include <string>
#include "../../SIM/basic/vector.h"

struct PhaseLagSweepData{
    float phase;
    float muE;
    float sigmaE;
    float sigmaR;
};
enum PhaseLagGraphType{
    muE_vs_phase,
    sE_vs_phase,
    sR_vs_phase,
    None
};
enum ConfigType{
    B, E, Gun, Sim
};
enum AvailableConfigIndex{
    emax, frequency, phaselag,
    epath, bpath, ppath, cpath,
    starttime, endtime, dt,
    guntime, _enum, multh,
    thcount, _r1, _r2, 
    ein, einstd,
    addmagnet, magrotation,
    bunchnum, gunperiod,
    targeten, output,
    debug,
    endofconfig
};

struct ConfigurationCell{
    ConfigType Type;
    std::string Value;
    AvailableConfigIndex Index;
    ConfigurationCell(ConfigType type, std::string value, AvailableConfigIndex index) : Type(type), Value(value), Index(index){}
};

static const std::vector<ConfigurationCell> configs = std::vector<ConfigurationCell>(
    {   ConfigurationCell(E,"emax", emax),              ConfigurationCell(Gun, "einmean", ein),
        ConfigurationCell(Gun, "einstd", einstd),
        ConfigurationCell(Gun,"targeten", targeten),   ConfigurationCell(E, "freq", frequency),
        ConfigurationCell(E,"phaselag", phaselag),      ConfigurationCell(Sim,"starttime", starttime),
        ConfigurationCell(Sim,"endtime", endtime),     ConfigurationCell(Sim,"dt", dt), 
        ConfigurationCell(Gun,"guntime", guntime),     ConfigurationCell(Gun,"gunperiod", gunperiod), 
        ConfigurationCell(Gun,"enum", _enum),          ConfigurationCell(Gun,"bunchnum", bunchnum),
        ConfigurationCell(E,"r1", _r1),                 ConfigurationCell(E,"r2", _r2), 
        ConfigurationCell(Sim,"epath", epath),         ConfigurationCell(Sim,"bpath", bpath), 
        ConfigurationCell(Sim,"cpath", cpath),         ConfigurationCell(Sim,"ppath", ppath), 
        ConfigurationCell(Sim,"multh",multh),          ConfigurationCell(Sim,"thcount",thcount),      
        ConfigurationCell(B,"magrotation", magrotation),ConfigurationCell(B,"addmagnet", addmagnet),    
});


/// @brief Description of single electron at time ElectronSnapshot.time
struct ElectronSnapshot{
    float time;
    float energy;
    vector3d position;
    vector3d velocity;
};
/// @brief Description of single electron over some time
struct ElectronLog{
    std::vector<ElectronSnapshot> time_slices;
    int e_index;
    int b_index;
};
/// @brief Description of the E vector at the point RFPoint.position, with constant time
struct RFPoint{
    vector3d position;
    vector3d field;
    float magnitude;
};
/// @brief Description of an E field at time RFSnapshot.time
struct RFSnapshot{
    float time;
    float max = 0; // in MV/s
    std::vector<RFPoint> field;
};
/// @brief Description of an E field over some time
struct RFLog{
    float RF_max = 0; // in MV/m
    std::vector<RFSnapshot> time_slices;
};
/// @brief Description of a static B field
struct StaticMagneticFieldLog{
    std::vector<std::pair<float, float>> non_zero_positions;
};


#endif