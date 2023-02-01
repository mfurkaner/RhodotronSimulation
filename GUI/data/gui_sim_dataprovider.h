#ifndef GUI_DATAPROVIDER_H
#define GUI_DATAPROVIDER_H


#include "../../SIM/basic/vector.h"


namespace RhodotronSimulatorGUI::data{

 
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
        std::vector<RFPoint> field;
    };

    /// @brief Description of an E field over some time
    struct RFLog{
        std::vector<RFSnapshot> time_slices;
    };

    /// @brief Description of a static B field
    struct StaticMagneticFieldLog{
        std::vector<std::pair<float, float>> non_zero_positions;
    };


    class DataProvider{
        bool _data_filled = false;

        std::string _elog_path = "xy/paths/";
        std::string _rflog_path = "xy/rf.txt";
        std::string _mlog_path = "xy/magnet.txt";

        std::vector<ElectronLog> _electrons_log;
        RFLog _rf;
        StaticMagneticFieldLog _magnets;

        uint32_t _enum;
        uint32_t _bnum;

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

    };


}


#endif