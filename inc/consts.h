#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>

#define CONSTS_H

const double c = 2.99792458e8; //m/s
const double E0=0.511;  // MeV
const double ns=1E-9; //ns conversion


//const double deg_to_rad = 0.01745;
extern uint64_t STEPS_TAKEN;
extern double GUN_ACTIVE_TIME; // ns
extern int NUM_OF_ELECTRONS;
extern bool NOTIFICATIONS;
extern int MAX_THREAD_COUNT;
extern bool MULTI_THREAD;

const double emass = 9.10938356e-31;    // kg
const double echarge = 1.60217662e-19;  //coulomb
const double eQMratio = -1.75882e11;    // C/kg

#define deg_to_rad  0.01745
#define US_FOR_A_STEP 0.06467

#define Emax 0.960000 // MVolts/m
#define freq 107.5    // MHz
#define Ein 40.0      // KeV
extern double dT;     // ns
extern double dT_out; // ns
#define R1 0.188241   // m
#define R2 0.752967   // m
#define Emax_pos R1*1000  // mm (Emax radial position)

#define MAGNET_ROTATION 5.0 //degrees
#define MAGNET_ROTATION_R MAGNET_ROTATION*deg_to_rad

#define dD 0.001       // m
#define PHASE_SWEEP 15 // degrees
#define MAGNET_TIME_SWEEP_MIN 2.0 // ns
#define MAGNET_TIME_SWEEP_MAX 9.0 // ns