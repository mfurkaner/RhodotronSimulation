#include "interactor.h"
#include "../basic/vector.h"


    vector3d Interactor::q_E_interaction_LF(Electron& e, const RFField& E, double time_interval){
        vector3d Efield = E.getEfield(e.pos);                            // Calculate E vector
        vector3d F_m = Efield*1E6*eQMratio;                           // Calculate F/m vector
        vector3d acc = (F_m - e.vel*(e.vel*F_m)*_inv_c_sq)*e.gamma_inv();     // Calculate a vector
        return acc;
    }
    vector3d Interactor::q_B_interaction_LF(Electron& e, const MagneticField& B, double time_interval){
        if (B.isInside(e.pos) == -1){
            return vector3d(0,0,0);
        }

        vector3d Bfield = B.getField(e.pos);                                          // Calculate B vector
        vector3d F_m = (e.vel % Bfield)*eQMratio;                                   // Calculate F/m vector
        vector3d acc = (F_m - e.vel*(e.vel*F_m)*_inv_c_sq)*e.gamma_inv();                   // Calculate a vector
        return acc;
    }
    vector3d Interactor::q_EM_interaction_LF(Electron& e, const RFField& E, const MagneticField& B, double time_interval){
        vector3d lf_E = q_E_interaction_LF(e, E, time_interval);
        vector3d lf_B = q_B_interaction_LF(e, B, time_interval);
        return lf_E + lf_B;
    }

    vector3d Interactor::q_E_interaction_RK(const Electron& e, const RFField& E, double time_interval){
        Electron e_dummy(e.pos, e.vel, 0);
        double dt_halved = time_interval*0.5;
        // get k1                                    // Calculate E vector
        vector3d F_m = E.getEfield(e_dummy.pos)*1E6*eQMratio;                                             // Calculate F/m vector
        vector3d k1 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
        // get k2
        e_dummy.move(dt_halved);
        e_dummy.accelerate(k1, dt_halved);
        F_m = E.getEfield(e_dummy.pos)*1E6*eQMratio;                                                      // Calculate F/m vector
        vector3d k2 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
        // get k3
        e_dummy.vel = e.vel;
        e_dummy.accelerate(k2, dt_halved);
        vector3d k3 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();      // Calculate a vector
        // get k4
        e_dummy.vel = e.vel;
        e_dummy.move(dt_halved);
        e_dummy.accelerate(k3, time_interval);
        F_m = E.getEfield(e_dummy.pos)*1E6*eQMratio;   
        vector3d k4 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();

        return (k1 + k2*2 + k3*2 + k4)/6;
    }

    vector3d Interactor::q_B_interaction_RK(const Electron& e, const MagneticField& B, double time_interval){
        if (B.isInside(e.pos) == -1){
            return vector3d(0,0,0);
        }
        Electron e_dummy(e.pos, e.vel, 0);
        double time_halved = time_interval*0.5;
        // get k1                                       
        vector3d F_m = (e_dummy.vel % B.getField(e.pos))*eQMratio;                                       
        vector3d k1 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();     
        // get k2
        e_dummy.move(time_halved);
        e_dummy.accelerate(k1, time_halved);                                              
        F_m = (e_dummy.vel % B.getField(e_dummy.pos))*eQMratio;                                               
        vector3d k2 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();    
        // get k3
        e_dummy.vel = e.vel;
        e_dummy.accelerate(k2, time_halved);
        vector3d k3 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();   
        // get k4
        e_dummy.vel = e.vel;
        e_dummy.move(time_halved);
        e_dummy.accelerate(k3, time_interval);                                           
        F_m = (e_dummy.vel % B.getField(e_dummy.pos))*eQMratio;  
        vector3d k4 = (F_m - e_dummy.vel*(e_dummy.vel*F_m)*_inv_c_sq)*e_dummy.gamma_inv();

        return (k1 + k2*2 + k3*2 + k4)/6;
    }

    vector3d Interactor::q_EM_interaction_RK(const Electron& e, const RFField& E, const MagneticField& B, double time_interval){
        vector3d run_kut_E = q_E_interaction_RK(e, E, time_interval);
        vector3d run_kut_B = q_B_interaction_RK(e, B, time_interval);
        return run_kut_E + run_kut_B;
    }