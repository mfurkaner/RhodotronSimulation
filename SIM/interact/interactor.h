#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "../particles/particle.h"
#include "../fields/fields.h"

class Interactor{
public:

    static vector3d q_E_interaction_LF(Electron& e, const RFField& E, double time_interval);
    static vector3d q_B_interaction_LF(Electron& e, const MagneticField& B, double time_interval);
    static vector3d q_EM_interaction_LF(Electron& e, const RFField& E, const MagneticField& B, double time_interval);

    static vector3d q_E_interaction_RK(const Electron& e, const RFField& E, double time_interval);
    static vector3d q_B_interaction_RK(const Electron& e, const MagneticField& B, double time_interval);
    static vector3d q_EM_interaction_RK(const Electron& e, const RFField& E, const MagneticField& B, double time_interval);

};




#endif