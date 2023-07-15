#ifndef FIELDS_H
    #include "fields.h"
#endif
#ifndef RHODO2D_H
    #include "rhodo2d.h"
#endif

void StaticConstantEField::SetE(vector3d E){
    this->E = E;
}

void StaticConstantEField::SetXY(double x, double y){
    this->x = x;
    this->y = y;
}


vector3d StaticConstantEField::GetE() const {return E;}

vector3d StaticConstantEField::actOn(Electron2D& e){
    if( (e.pos.X() > x || e.pos.X() < -x ) || (e.pos.Y() > y || e.pos.Y() < -y)){
        return vector3d(0,0,0);
    }
    vector3d F_m = E*1E6*eQMratio;                           // Calculate F/m vector
    vector3d acc = (F_m - e.vel*(e.vel*F_m)/(c*c))/e.gamma();     // Calculate a vector
    return acc;
}

int StaticConstantEField::log(DataStorage& rf){
    int count = 0;
    for( double i = -R2; i <= R2 ; i += 0.05){
        for (double j = -R2; j <= R2 ; j += 0.05){
            vector3d pos(i,j,0);
            if ( pos.magnitude() > R1 ){
                rf << "p: " << pos << "  E: " << E << "   mag: " << E.magnitude() <<"\n";
                count ++;
            }
        }
    }
    return count;
}

void RFField::update(double time){
    double w = 2 * M_PI * frequency / 1000;
    E = sin(w*time + phase_lag*deg_to_rad)*E_max;
}

double RFField::E_radial(double R){
    double E_zero = E*Emax_pos/R;
    double res = E_zero*((R < -R1*1000)*(R > -R2*1000) | (R < R2*1000)*(R > R1*1000));
    return res;
}

vector3d RFField::getField(vector3d position){
    double radial = E_radial( position.magnitude()*1000 );
    return position.direction() * radial;
}

double RFField::getField( double R ){
    return E_radial(R*1000);
}

int RFField::log( DataStorage& rf , double time){
    int count = 0;
    for( double i = -R2; i <= R2 ; i += 0.05){
        for (double j = -R2; j <= R2 ; j += 0.05){
            vector3d pos(i,j,0);
            vector3d Efield = getField(pos);
            if ( pos.magnitude() > R1 ){
                rf << "t: " << time << " p: " << pos << "  E: " << Efield << "   mag: " << Efield.magnitude() <<"\n";
                count ++;
            }
        }
    }
    return count;
}

vector3d RFField::actOn(Electron2D& e){
    vector3d Efield = getField(e.pos);                            // Calculate E vector
    vector3d F_m = Efield*1E6*eQMratio;                           // Calculate F/m vector
    vector3d acc = (F_m - e.vel*(e.vel*F_m)/(c*c))/e.gamma();     // Calculate a vector
    return acc;
}

double Magnet::getOptimalB(double E, double minB, double maxB, double stepsize){
    Electron2D e;
    e.Et = E + E0;
    double mag = e.get_vel();
    double min_r = R2;
    double optB = 0;

    for(double B = minB; B < maxB ; B += stepsize){
        e.pos = vector3d(R1,0,0);
        e.vel = vector3d(mag, 0, 0);
        MagneticField mf;
        mf.addMagnet(B, this->r, this->position);
        for(double t = 0; t < 10 ; t+=0.001){
            vector3d acc_B = mf.actOn(e);
            e.accelerate(acc_B , 0.001);
            e.move(acc_B, 0.001);
            if(e.pos.magnitude() < min_r){
                min_r = e.pos.magnitude();
                optB = B;
            }
        }
    }
    cout << min_r << "  ";
    return optB;
}

bool isInsideHalfSphere(vector3d e_position, double r, vector3d hs_position){
    vector3d relative = e_position - hs_position;                                       // get the relative position of electron with respect to magnet center
    if ( relative.magnitude() <= r && relative * hs_position.direction() >= 0){          // if the relative distance is less than r && relative doesn't have opposite direction
        return true;                                                                    // electron is inside the half sphere
    }
    return false;
}

int MagneticField::isInside(vector3d position){
    for(int i = 0; i < magnets.size() ; i++){
        if ( isInsideHalfSphere(position, magnets[i].r, magnets[i].position ) ){
            return i;
        }
    }
    return -1;
}

void MagneticField::addMagnet(double B, double r, vector3d position){
    magnets.push_back(Magnet(B, r, position));
    hasEntered.push_back(false);
}

void MagneticField::addMagnet(Magnet m){
    magnets.push_back(m);
    hasEntered.push_back(false);
}

vector3d MagneticField::getField(vector3d position){
    int magnet_index = isInside(position);
    if ( magnet_index == -1 ){
        return vector3d(0, 0, 0);
    }
    if ( !hasEntered[magnet_index] ){ 
        hasEntered[magnet_index] = true; 
        vector3d v = position - magnets[magnet_index].position;
        relativeEnterDistance.push_back(v.magnitude());
    }
    return vector3d(0, 0, magnets[magnet_index].B);
}

vector3d MagneticField::actOn(Electron2D& e, bool log){
    if (isInside(e.pos) == -1){
        return vector3d(0,0,0);
    }

    vector3d Bfield = getField(e.pos);                                          // Calculate B vector
    vector3d F_m = (e.vel % Bfield)*eQMratio;                                   // Calculate F/m vector
    vector3d acc = (F_m)/e.gamma();                   // Calculate a vector
    if(log){
        cout << "Fm = "<< F_m.magnitude() <<" (" << F_m.X() << " , " << F_m.Y() << " , " << F_m.Z();
    }
    return acc;
}

std::vector<vector3d> MagneticField::actOnAndGetRungeKuttaCoef(Electron2D& e, double dt){
    vector<vector3d> result;
    result.push_back(e.pos);
    result.push_back(e.vel);
    if (isInside(e.pos) == -1){
        e.move(dt);
        result[0] = e.pos;
        return result;
    }
    Electron2D e_dummy;
    e_dummy.Et = e.Et;
    e_dummy.pos = e.pos;
    e_dummy.vel = e.vel;
    Electron2D e_dummy2;
    e_dummy2.Et = e.Et;
    e_dummy2.pos = e.pos;
    e_dummy2.vel = e.vel;
    // get k1
    vector3d Bfield = getField(e.pos);                                             
    vector3d F_m = (e_dummy.vel % Bfield)*eQMratio;         
    vector3d a1 = F_m/e_dummy.gamma();                              
    e_dummy2.move(dt);
    e_dummy2.accelerate(a1, dt);
    vector<vector3d> k1;  
    k1.push_back(e_dummy2.pos); k1.push_back(e_dummy2.vel);

    // get k2
    e_dummy.move(dt/2);
    e_dummy.accelerate(a1, dt/2);

    e_dummy2.Et = e_dummy.Et;
    e_dummy2.pos = e_dummy.pos;
    e_dummy2.vel = e_dummy.vel;

    Bfield = getField(e_dummy.pos);                                                 
    F_m = (e_dummy.vel % Bfield)*eQMratio;                                               
    vector3d a2 = F_m/e_dummy.gamma();                              
    e_dummy2.move(dt);
    e_dummy2.accelerate(a2, dt);
    vector<vector3d> k2;  
    k2.push_back(e_dummy2.pos); k2.push_back(e_dummy2.vel);  

    // get k3
    e_dummy.vel = e.vel;
    e_dummy.accelerate(a2, dt/2);
    
    e_dummy2.Et = e_dummy.Et;
    e_dummy2.pos = e_dummy.pos;
    e_dummy2.vel = e_dummy.vel;

    F_m = (e_dummy.vel % Bfield)*eQMratio;                                               
    vector3d a3 = F_m/e_dummy.gamma();                              
    e_dummy2.move(dt);
    e_dummy2.accelerate(a3, dt);
    vector<vector3d> k3;  
    k3.push_back(e_dummy2.pos); k3.push_back(e_dummy2.vel);  
    
    // get k4
    e_dummy.vel = e.vel;
    e_dummy.move(dt/2);
    e_dummy.accelerate(a3, dt);
    Bfield = getField(e_dummy.pos);                                                 
    F_m = (e_dummy.vel % Bfield)*eQMratio;  
    vector3d a4 = F_m/e_dummy.gamma();                              
    e_dummy.move(dt);
    e_dummy.accelerate(a4, dt);
    vector<vector3d> k4;  
    k4.push_back(e_dummy2.pos); k4.push_back(e_dummy2.vel);  

    result[0] = (k1[0] + k2[0]*2 + k3[0]*2 + k4[0])/6;
    result[1] = (k1[1] + k2[1]*2 + k3[1]*2 + k4[1])/6;
    return result;
}

vector3d MagneticField::getJerk(vector3d pos, vector3d vel, vector3d acc){
    if(acc == vector3d(0,0,0)){
        return vector3d(0,0,0);
    }
    vector3d Bfield = getField(pos);   
    vector3d F_m = (vel % Bfield)*eQMratio;        
    vector3d derF_m = (acc % Bfield)*eQMratio;
    double A = acc * vel / (c*c);
    A *= -2*(1 - vel*vel/(c*c));
    return (vel*(acc * F_m)/(c*c) + vel*(vel*derF_m)/(c*c) + acc*(vel*F_m)/(c*c) - derF_m)*A;
}

void MagneticField::log(DataStorage& magnet){
    for(double x = -2; x <= 2 ; x+=0.001){
        for(double y = -2; y <= 2; y+=0.001){
            vector3d v(x,y,0);
            if ( getField(v).Z() != 0 ){
                magnet << x << "  " << y << "\n";
            }
        }
    }
}


double rho_to_magnetic_field(double rho, double Ek){
    double B = c/(eQMratio*rho);
    double gamma = (Ek + E0)/E0;
    B *= gamma*sqrt(1- 1/(gamma*gamma));
    return B;
}

double magnetic_field_to_rho(double B, double Ek){
    double rho = c/(eQMratio*B);
    double gamma = (Ek + E0)/E0;
    rho *= gamma*sqrt(1- 1/(gamma*gamma));
    return rho;
}