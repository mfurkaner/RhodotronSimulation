#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>



#ifndef CONSTS_H
#define CONSTS_H

const double c = 2.99792458e8; //m/s
const double c_sq = c*c;
const double _inv_c = 1.0/c;    
const double _inv_c_sq = 1.0/c_sq;
const double E0 = 0.51099895;  // MeV
const double E0_sq = E0*E0;
const double ns = 1E-9; //ns conversion
const double emass = 9.10938356e-31;    // kg
const double echarge = 1.60217662e-19;  //coulomb
const double eQMratio = -1.75882001076e11;    // C/kg

#define ROTATION_ZERO_LIMIT 1.0E-07
#define deg_to_rad  M_PI/180

//#define R1 0.188241   // m
//#define R2 0.752967   // m


#endif