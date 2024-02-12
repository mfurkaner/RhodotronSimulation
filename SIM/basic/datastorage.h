#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <fstream>
class DataStorage{
    public:
    std::ofstream fout;
    std::string filepath;
    bool fileIsOpen = false;


    DataStorage(std::string filepath):filepath(filepath){}
    DataStorage(const DataStorage& other);
    ~DataStorage();

    void open();
    void close();

    template<typename T>
    DataStorage& operator<<(T in){
        fout << in;
        return *this;
    }
};

#endif