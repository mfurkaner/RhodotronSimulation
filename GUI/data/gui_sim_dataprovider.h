#ifndef GUI_DATAPROVIDER_H
#define GUI_DATAPROVIDER_H



#include "gui_data_types.h"


    class DataProvider{
        bool _data_filled = false;

        std::string _elog_path = "xy/paths/";
        std::string _rflog_path = "xy/rf.txt";
        std::string _mlog_path = "xy/magnet.txt";

        std::vector<ElectronLog> _electrons_log;
        float _electron_time_step = 0.1f;

        RFLog _rf;
        float _efield_time_step = 0.1f;
            
        StaticMagneticFieldLog _magnets;

        uint32_t _enum;
        uint32_t _bnum;
        float _Emax = 0;

        void _fillElectrons();
        void _fillEField();
        void _fillBField();

    public: 

        void SetEnum(int _enum_) {_enum = _enum_;}
        void SetBnum(int _bnum_) {_bnum = _bnum_;}
        
        void SetElogPath(std::string elog_path){ _elog_path = elog_path;}
        void SetRflogPath(std::string rflog_path){ _rflog_path = rflog_path;}
        void SetStaticBfieldlogPath(std::string mlog_path){ _mlog_path = mlog_path;}

        void fillLogs();
        void clearLogs();
        bool isDataFilled(){return _data_filled;}

        const std::vector<ElectronLog>& GetElectrons();
        const RFLog& GetEField();
        const StaticMagneticFieldLog& GetStaticBField();

        const float GetEmax(){return _Emax;}

        const float GetElectronTimeStep();
        const float GetEFieldTimeStep();
    };


//}


#endif