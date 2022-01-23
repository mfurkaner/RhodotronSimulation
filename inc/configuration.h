#include <iostream>
#include <fstream>
#ifndef GNUPLOT_H
    #include "gnuplot.h"
#endif
#define CONFIGURATION_H

enum AvailableConfigIndex{
    emax, frequency, phaselag,
    epath, bpath, ppath, cpath,
    starttime, endtime, dt,
    guntime, _enum, multh,
    thcount, _r1, _r2, ein,
    addmagnet, magrotation,
    bunchnum, gunperiod,
    targeten,

    endofconfig
};

class Configuration{
private:
    std::string available_config[endofconfig] = {
        "emax", "freq", "phaselag", 
        "epath", "bpath", "ppath", "cpath",
        "starttime", "endtime", "dt", 
        "guntime", "enum", "multh", 
        "thcount", "r1", "r2", "ein", 
        "addmagnet", "magrotation",
        "bunchnum", "gunperiod",
        "targeten"
        };
    std::ifstream fin;
    std::string filepath;
    bool fileIsOpen = false;

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
    double mag_rotation;    bool magRotIsSet = false;
    double target_energy = 2 ; bool targetEnSet = false;

    double magB;
    double magR;
    double magdist;
    vector3d magpos;
public:
    vector<Magnet> magnets;
    Configuration(std::string filepath):filepath(filepath){}

    void print(){
        cout << "\n -- Simulation Configuration --\n";
        if (isEmaxSet()) cout << "Emax : " << Emax << "\tMV/m\n";
        if (isFreqSet()) cout << "Freq : " << freq << "\tMHz\n";
        if (isPhaseLagSet()) cout << "Phase Lag : " << phase_lag << "\tdegree\n";
        if (isEPathSet()) cout << "E_path : " << e_path << "\n";
        if (isBPathSet()) cout << "B_path : " << b_path << "\n";
        if (isPPathSet()) cout << "P_path : " << p_path << "\n";
        if (isCPathSet()) cout << "C_path : " << c_path << "\n";
        if (isSTimeSet()) cout << "StartTime : " << stime << "\tns\n";
        if (isETimeSet()) cout << "EndTime : " << etime << "\tns\n";
        if (isdTSet()) cout << "dT : " << dT << "\tns\n";
        if (isGunTimeSet()) cout << "guntime : " << gun_time << "\tns\n";
        if (isGunPeriodSet()) cout << "gunperiod : " << gun_period << "\tns\n";
        if (isNumOfESet()) cout << "enum : " << num_of_e << "\n";
        if (isNumOfBunchSet()) cout << "bunchnum : " << num_of_bunch << "\n";
        if (isMTSet()) cout << "MT : " << multi_thread << "\n";
        if (isR1Set()) cout << "R1 : " << r1 << "\tm\n";
        if (isR2Set()) cout << "R2 : " << r2 << "\tm\n";
        if (magnets.size()) cout << "Magnet count :\t" << magnets.size() << "\n";
        if (isEinSet()) cout << "Ein : " << Ein << "\tMeV\n";
        if (isTargetEnSet()) cout << "TargetE : " << target_energy << "\tMeV\n";
        cout << "--------------------------------\n\n";
    }

    double getEmax(){return Emax;}
    double getFreq(){return freq;}
    double getPhaseLag(){return phase_lag;}
    std::string getEpath(){return e_path;}
    std::string getBpath(){return b_path;}
    std::string getPpath(){return p_path;}
    std::string getCpath(){return c_path;}
    double getSTime(){return stime;}
    double getETime(){return etime;}
    double getdT(){return dT;}
    double getGunTime(){return gun_time;}
    double getGunPeriod(){return gun_period;}
    double getNumOfE(){return num_of_e;}
    double getNumOfB(){return num_of_bunch;}
    bool getMultiThread(){return multi_thread;}
    unsigned int getThCount(){return th_count;}
    double getR1(){return r1;}
    double getR2(){return r2;}
    double getEin(){return Ein;}
    double getTargetEnergy(){return target_energy;}

    bool isEmaxSet(){return EmaxIsSet;}
    bool isFreqSet(){return freqIsSet;}
    bool isPhaseLagSet(){return phaseLagIsSet;}
    bool isEPathSet(){return epathIsSet;}
    bool isBPathSet(){return bpathIsSet;}
    bool isPPathSet(){return ppathIsSet;}
    bool isCPathSet(){return cpathIsSet;}
    bool isSTimeSet(){return sTimeIsSet;}
    bool isETimeSet(){return eTimeIsSet;}
    bool isdTSet(){return dTIsSet;}
    bool isGunTimeSet(){return gunTimeIsSet;}
    bool isGunPeriodSet(){return gunPeriodIsSet;}
    bool isNumOfESet(){return enumIsSet;}
    bool isNumOfBunchSet(){return bnumIsSet;}
    bool isMTSet(){return mtIsSet;}
    bool isThCountSet(){return thCountIsSet;}
    bool isR1Set(){return r1IsSet;}
    bool isR2Set(){return r2IsSet;}
    bool isEinSet(){return EinIsSet;}
    bool isTargetEnSet(){return targetEnSet;}
    bool areThereMagnets(){return magnets.size() != 0;}

    void logConfiguration(std::string logpath){
        fin.open(filepath, std::ios::in);
        std::string line;
        DataStorage configStorage(logpath);
        configStorage.open();
        do{
            std::getline(fin, line);
            configStorage << line << "\n";
        }while( !fin.eof() );
        fin.close();
        configStorage.close();
    }


    void getConfiguration(){
        fin.open(filepath, std::ios::in);
        std::string cmd;
        do{
            std::getline(fin, cmd);
            int i = 0;
            for(; i < endofconfig ; i++){
                if (cmd.find("#", 0) != std::string::npos){ i = -1; break; }
                else if (cmd.find(available_config[i], 0) != std::string::npos) {
                    break;
                }
            }
            switch (i)
            {
            case emax:
                Emax = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                EmaxIsSet = true;
                break;
            case frequency:
                freq = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                freqIsSet = true;
                break;
            case phaselag:
                phase_lag = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                phaseLagIsSet = true;
                break;
            case epath:
                cmd = cmd.substr( cmd.find('=', 0) + 1, 50);
                cmd.erase(remove(cmd.begin(), cmd.end(), ' '), cmd.end());
                e_path = cmd;
                epathIsSet = true;
                break;
            case bpath:
                cmd = cmd.substr( cmd.find('=', 0) + 1, 50);
                cmd.erase(remove(cmd.begin(), cmd.end(), ' '), cmd.end());
                b_path = cmd;
                bpathIsSet = true;
                break;
            case cpath:
                cmd = cmd.substr( cmd.find('=', 0) + 1, 50);
                cmd.erase(remove(cmd.begin(), cmd.end(), ' '), cmd.end());
                c_path = cmd;
                cpathIsSet = true;
                break;
            case starttime:
                stime = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                sTimeIsSet = true;
                break;
            case endtime:
                etime = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() ) ;
                eTimeIsSet = true;
                break;
            case dt:
                dT = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                dTIsSet = true;
                break;
            case guntime: 
                gun_time = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                gunTimeIsSet = true;
                break;
            case _enum: 
                num_of_e = atoll( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                enumIsSet = true;
                break;
            case multh:
                multi_thread = atoi( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                mtIsSet = true;
                break;
            case thcount: 
                th_count = atoi( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                thCountIsSet = true;
                break;
            case _r1:
                r1 = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                r1IsSet = true;
                break;
            case _r2:
                r2 = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                r2IsSet = true;
                break;
            case ein:
                Ein = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                EinIsSet = true;
                break;
            case addmagnet:
                if(!magRotIsSet){ break; }
                magB = atof( cmd.substr( cmd.find('=', 0) + 1, cmd.find(',', 0) - 1).c_str() );
                cmd = cmd.substr( cmd.find(',', 0) + 1, 50);
                magR = atof( cmd.substr(0, cmd.find(',', 0) - 1).c_str() );
                cmd = cmd.substr( cmd.find('<', 0) + 1, cmd.find('>', 0) - 1);
                magdist = atof ( cmd.c_str() );
                magpos = vector3d( -magdist, 0, 0);
                magpos.rotate(vector3d(0,0,1), (180 - 2*mag_rotation) * ( magnets.size()+1 ) + mag_rotation);
                magnets.push_back(Magnet(magB, magR, magpos));
                break;
            case magrotation:
                mag_rotation = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                magRotIsSet = true;
                break;
            case bunchnum:
                num_of_bunch = atoll( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                bnumIsSet = true;
                break;
            case gunperiod:
                gun_period = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                gunPeriodIsSet = true;
                break;
            case targeten:
                target_energy = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                targetEnSet = true;
                break;
            default:
                break;
            }
        } while( !fin.eof() );
        fin.close();
    }

    

};