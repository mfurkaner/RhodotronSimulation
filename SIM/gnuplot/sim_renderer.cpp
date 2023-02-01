#include "sim_renderer.h"


void SimRenderer::Render(Configuration& config){
    gp.setRange(-1.5,1.5,-1.5,1.5);
    gp.enableMinorTics();
    gp.setCbRange(config.getEin(), config.getTargetEnergy());
    gp.setCbTic(0.1);
    gp.setRatio(1);
    gp.addCommand("set isosamples 500,500");
    gp.addCommand("set cblabel \"Energy(MeV)\" offset 1,0,0");
    gp.addCommand("set palette rgb 33,13,10");
    gp.addCommand("set terminal gif animate delay 3");
    gp.addCommand("set output \""+ config.getOutput() + "\"");
    gp.addCommand("set key top left");
    
    std::string plotCommand = "do for [i=1:" + std::to_string(config.getETime()*10 - 1) + "] ";
    plotCommand += "{plot \"xy/rf.txt\" every ::(i*916 - 915)::(i*916) using 1:2:($3/15):($4/15) '%*lf,( %lf ; %lf ; %*lf ),( %lf ; %lf ; %*lf ),%lf' title \"RF Field\" with vectors lc 6 head filled,";
    plotCommand += (config.areThereMagnets() ) ? "\"xy/magnet.txt\" u 1:2 title \"magnets\" ls 5 lc 4 ps 0.2, " : "";                    // 4=sari
    for( int j = 1 ; j <= config.getNumOfB() ; j++){
        for ( int i = 1 ; i <= config.getNumOfE(); i++)
        plotCommand +=  "\"xy/paths/b" + std::to_string(j) + "_e" + std::to_string(i) +".dat\" every ::i::i u 2:3:1 '%*lf,%lf,( %lf ; %lf ; %*lf ),( %*lf ; %*lf ; %*lf )' " + (j==1 && i == 1 ? "title \"bunch\"" : "notitle") + " ls 7 ps 0.5 palette, ";
    }
    plotCommand += "}";
    gp.setPlotCommand(plotCommand);

    gp.executeCommands();
    gp.plot();
    gp.waitUntilDone();
}