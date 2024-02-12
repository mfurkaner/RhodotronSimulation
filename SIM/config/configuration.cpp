#include "configuration.h"

#include <iostream>
#include <fstream>
#include "../basic/vector.h"
#include "../fields/fields.h"

using namespace std;


    double Configuration::getEmax(){return Emax;}
    double Configuration::getFreq(){return freq;}
    double Configuration::getPhaseLag(){return phase_lag;}
    std::string Configuration::getEpath(){return e_path;}
    std::string Configuration::getBpath(){return b_path;}
    std::string Configuration::getPpath(){return p_path;}
    std::string Configuration::getCpath(){return c_path;}
    double Configuration::getSTime(){return stime;}
    double Configuration::getETime(){return etime;}
    double Configuration::getdT(){return dT;}
    double Configuration::getGunTime(){return gun_time;}
    double Configuration::getGunPeriod(){return gun_period;}
    double Configuration::getNumOfE(){return num_of_e;}
    double Configuration::getNumOfB(){return num_of_bunch;}
    bool Configuration::getMultiThread(){return multi_thread;}
    unsigned int Configuration::getThCount(){return th_count;}
    double Configuration::getR1(){return r1;}
    double Configuration::getR2(){return r2;}
    double Configuration::getEin(){return Ein;}
    double Configuration::getEinStd(){return EinStd;}
    double Configuration::getTargetEnergy(){return target_energy;}
    std::string Configuration::getOutput(){return _output;}

    bool Configuration::isEmaxSet(){return EmaxIsSet;}
    bool Configuration::isFreqSet(){return freqIsSet;}
    bool Configuration::isPhaseLagSet(){return phaseLagIsSet;}
    bool Configuration::isEPathSet(){return epathIsSet;}
    bool Configuration::isBPathSet(){return bpathIsSet;}
    bool Configuration::isPPathSet(){return ppathIsSet;}
    bool Configuration::isCPathSet(){return cpathIsSet;}
    bool Configuration::isSTimeSet(){return sTimeIsSet;}
    bool Configuration::isETimeSet(){return eTimeIsSet;}
    bool Configuration::isdTSet(){return dTIsSet;}
    bool Configuration::isGunTimeSet(){return gunTimeIsSet;}
    bool Configuration::isGunPeriodSet(){return gunPeriodIsSet;}
    bool Configuration::isNumOfESet(){return enumIsSet;}
    bool Configuration::isNumOfBunchSet(){return bnumIsSet;}
    bool Configuration::isMTSet(){return mtIsSet;}
    bool Configuration::isThCountSet(){return thCountIsSet;}
    bool Configuration::isR1Set(){return r1IsSet;}
    bool Configuration::isR2Set(){return r2IsSet;}
    bool Configuration::isEinSet(){return EinIsSet;}
    bool Configuration::isEinStdSet(){return EinStdIsSet;}
    bool Configuration::isTargetEnSet(){return targetEnSet;}
    bool Configuration::isOutputSet(){return outputIsSet;}
    bool Configuration::areThereMagnets(){return magnets.size() != 0;}
    bool Configuration::debugMode(){return debug_mode;}

    void Configuration::print(){
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
        if (isMTSet()) cout << "thcount : " << th_count << "\n";
        if (isR1Set()) cout << "R1 : " << r1 << "\tm\n";
        if (isR2Set()) cout << "R2 : " << r2 << "\tm\n";
        if (magnets.size()) cout << "Magnet count :\t" << magnets.size() << "\n";
        if (isEinSet()) cout << "Ein : " << Ein << "\tMeV\n";
        if (isEinStdSet()) cout << "EinStd : " << EinStd << "\tMeV\n";
        if (isTargetEnSet()) cout << "TargetE : " << target_energy << "\tMeV\n";
        if (isOutputSet()) cout << "Output : " << _output << "\n";
        cout << "--------------------------------\n\n";
        return;
    }

    void Configuration::logConfiguration(std::string logpath){
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


    void Configuration::getConfiguration(){
        fin.open(filepath, std::ios::in);
        std::string cmd;
        do{
            std::getline(fin, cmd);
            int i = 0;
            for(; i < endofconfig ; i++){
                if (cmd.find("#", 0) != std::string::npos){ 
                    i = -1;
                    break; 
                }
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
                #ifdef __APPLE__
                    cmd.erase(remove(cmd.begin(), cmd.end(), ' '), cmd.end());
                #endif
                e_path = cmd;
                epathIsSet = true;
                break;
            case bpath:
                cmd = cmd.substr( cmd.find('=', 0) + 1, 50);
                #ifdef __APPLE__
                    cmd.erase(remove(cmd.begin(), cmd.end(), ' '), cmd.end());
                #endif
                b_path = cmd;
                bpathIsSet = true;
                break;
            case cpath:
                cmd = cmd.substr( cmd.find('=', 0) + 1, 50);
                #ifdef __APPLE__
                    cmd.erase(remove(cmd.begin(), cmd.end(), ' '), cmd.end());
                #endif
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
                mtIsSet = true;
                multi_thread = true;
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
            case ein_std:
                EinStd = atof( cmd.substr( cmd.find('=', 0) + 1, 20).c_str() );
                EinStdIsSet = true;
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
            case output:
                cmd = cmd.substr( cmd.find('=', 0) + 1, 50);
                #ifdef __APPLE__
                    cmd.erase(remove(cmd.begin(), cmd.end(), ' '), cmd.end());
                #endif
                _output = cmd;
                outputIsSet = true;
                break;
            case debug:
                debug_mode = true;
                break;
            default:
                break;
            }
        } while( !fin.eof() );
        fin.close();
    }