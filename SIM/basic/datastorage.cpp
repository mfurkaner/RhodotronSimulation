#include "datastorage.h"

    struct ParticleLog;
    class vector3d;

    std::ostream& operator<<(std::ostream& stream, vector3d& vect); 
    std::ostream& operator<<(std::ostream& stream, ParticleLog& log);

    DataStorage::DataStorage(const DataStorage& other){
        filepath = other.filepath;
        fileIsOpen = other.fileIsOpen;
    }
    DataStorage::~DataStorage(){ if (fileIsOpen) fout.close();}

    void DataStorage::open(){
        fout.open(filepath, std::ios::out);
        fileIsOpen = true;
    }
    void DataStorage::close(){
        fout.close();
        fileIsOpen = false;
    }

    