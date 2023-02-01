#ifndef SIM_RENDERER_H
#define SIM_RENDERER_H

#include "gnuplot.h"
#include "../config/configuration.h"


class SimRenderer{
    Gnuplot gp;

public:
    
    void Render(Configuration& config);

};

#endif