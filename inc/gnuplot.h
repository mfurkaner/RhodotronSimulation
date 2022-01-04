#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

#define GNUPLOT_H

class DataStorage{
    std::ofstream fout;
    std::string filepath;
    bool fileIsOpen = false;

    public:
    DataStorage(std::string filepath):filepath(filepath){}
    ~DataStorage(){ if (fileIsOpen) fout.close();}

    void open(){
        fout.open(filepath, std::ios::out);
        fileIsOpen = true;
    }
    void close(){
        fout.close();
        fileIsOpen = false;
    }

    template<typename T>
    DataStorage& operator<<(T in){
        fout << in;
        return *this;
    }

};

class Gnuplot{
    FILE *gnupipe = NULL;
    std::vector<std::string> commands;
    std::string filepath;

public:
    Gnuplot(){
        gnupipe = popen("gnuplot -persistent", "w");
    }
    
    void setFilePath(std::string filepath){
        this->filepath = filepath;
    }

    void setRange(double x_min, double x_max, double y_min, double y_max){
        std::string range = "set xrange [";
        range += std::to_string(x_min);
        range += ":";
        range += std::to_string(x_max);
        range += "]; set yrange [";
        range += std::to_string(y_min);
        range += ":";
        range += std::to_string(y_max);
        range += "]";
        commands.push_back(range);
    }

    void setRatio(double num){
        std::string rat = "set size ratio ";
        rat += std::to_string(num); 
        commands.push_back(rat);
    }

    void addCommand(std::string command){
        commands.push_back(command);
    }

    void executeCommands(){
        for(int i = 0; i < commands.size(); i++){
            fprintf(gnupipe, "%s\n", commands[i].c_str() );
        }
    }

};