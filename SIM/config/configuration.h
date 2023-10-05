
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <fstream>
#include "../basic/vector.h"
#include "../basic/datastorage.h"
#include "../fields/fields.h"

using namespace std;


enum AvailableConfigIndex{
    emax, frequency, phaselag,
    epath, bpath, ppath, cpath,
    starttime, endtime, dt,
    guntime, _enum, multh,
    thcount, _r1, _r2, 
    ein, ein_std,
    addmagnet, magrotation,
    bunchnum, gunperiod,
    targeten, output,
    debug,

    endofconfig
};

class Configuration{
private:
    std::string available_config[endofconfig] = {
        "emax", "freq", "phaselag", 
        "epath", "bpath", "ppath", "cpath",
        "starttime", "endtime", "dt", 
        "guntime", "enum", "multh", 
        "thcount", "r1", "r2", 
        "einmean", "einstd", 
        "addmagnet", "magrotation",
        "bunchnum", "gunperiod",
        "targeten", "output",
        "debug"
        };
    std::ifstream fin;
    std::string filepath;

    double Emax;            bool EmaxIsSet = false;
    double freq;            bool freqIsSet = false;
    double phase_lag;       bool phaseLagIsSet = false;
    std::string e_path;     bool epathIsSet = false;
    std::string b_path;     bool bpathIsSet = false;
    std::string p_path;     bool ppathIsSet = false;
    std::string c_path;     bool cpathIsSet = false;
    double stime = 0;           bool sTimeIsSet = false;
    double etime;           bool eTimeIsSet = false;
    double dT;              bool dTIsSet = false;
    double gun_time;        bool gunTimeIsSet = false;
    double gun_period;  bool gunPeriodIsSet = false;
    double num_of_e;        bool enumIsSet = false;
    double num_of_bunch;    bool bnumIsSet = false;
    bool multi_thread;      bool mtIsSet = false;
    unsigned int th_count;  bool thCountIsSet = false;
    double r1;              bool r1IsSet = false;
    double r2;              bool r2IsSet = false;
    double Ein;             bool EinIsSet = false;
    double EinStd;           bool EinStdIsSet = false;
    double mag_rotation;    bool magRotIsSet = false;
    double target_energy = 2 ; bool targetEnSet = false;
    std::string _output = "out.gif" ; bool outputIsSet = false;
    bool debug_mode = false;

    double magB;
    double magR;
    double magdist;
    vector3d magpos;
public:
    std::vector<Magnet> magnets;
    Configuration(std::string filepath):filepath(filepath){}

    void print();

    double getEmax();
    double getFreq();
    double getPhaseLag();
    std::string getEpath();
    std::string getBpath();
    std::string getPpath();
    std::string getCpath();
    double getSTime();
    double getETime();
    double getdT();
    double getGunTime();
    double getGunPeriod();
    double getNumOfE();
    double getNumOfB();
    bool getMultiThread();
    unsigned int getThCount();
    double getR1();
    double getR2();
    double getEin();
    double getEinStd();
    double getTargetEnergy();
    std::string getOutput();

    bool isEmaxSet();
    bool isFreqSet();
    bool isPhaseLagSet();
    bool isEPathSet();
    bool isBPathSet();
    bool isPPathSet();
    bool isCPathSet();
    bool isSTimeSet();
    bool isETimeSet();
    bool isdTSet();
    bool isGunTimeSet();
    bool isGunPeriodSet();
    bool isNumOfESet();
    bool isNumOfBunchSet();
    bool isMTSet();
    bool isThCountSet();
    bool isR1Set();
    bool isR2Set();
    bool isEinSet();
    bool isEinStdSet();
    bool isTargetEnSet();
    bool isOutputSet();
    bool areThereMagnets();
    bool debugMode();

    void logConfiguration(std::string logpath);
    void getConfiguration();

};

#endif 