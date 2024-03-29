#ifndef GNUPLOT_H
#define GNUPLOT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <string.h>
#include "../basic/datastorage.h"

class Gnuplot{
    FILE *gnupipe = NULL;
    std::vector<std::string> commands;
    std::string plotcommand;
    std::string filepath;

public:
    Gnuplot(){
        gnupipe = popen("gnuplot -persistent", "w");
    }
    
    void setFilePath(std::string filepath){
        this->filepath = filepath;
    }

    void setPlotCommand(std::string plotcommand){
        this->plotcommand = plotcommand;
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

    void setCbRange(double min, double max){
        std::string cbrange = "set cbrange [";
        cbrange += std::to_string(min);
        cbrange += ":";
        cbrange += std::to_string(max);
        cbrange += "]";
        commands.push_back(cbrange);
    }

    void enableMinorTics(){
        commands.push_back("set mxtics");
        commands.push_back("set mytics");
    }

    void enableGrid(){
        commands.push_back("set grid");
    }

    void setCbTic(double ticLen){
        commands.push_back("set cbtics " + std::to_string(ticLen));
    }
    
    void disableLegend(){
        commands.push_back("unset key");
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

    void plot(){
        fprintf(gnupipe, "%s\n", plotcommand.c_str());
    }

    void waitUntilDone(){
        //std::cout << "Rendering the simulation results.\n"; 
        pclose(gnupipe);
    }

};

#endif