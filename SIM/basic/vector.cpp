#ifndef VECTOR_CPP
#define VECTOR_CPP
#include "vector.h"
#include "consts.h"
#include <math.h>

vector3d vector3d::operator+ (vector3d other){
    vector3d new_vec(this->x + other.x, this->y + other.y, this->z + other.z);
    return new_vec;
}

void vector3d::operator+= (vector3d other){
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
}

vector3d vector3d::operator- (vector3d other){
    vector3d new_vec(this->x - other.x, this->y - other.y, this->z - other.z);
    return new_vec;
}

void vector3d::operator-= (vector3d other){
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
}

vector3d vector3d::operator* (double num){
    vector3d new_vec(this->x * num, this->y * num, this->z * num);
    return new_vec;
}

void vector3d::operator*= (double num){
    this->x *= num;
    this->y *= num;
    this->z *= num;
}

vector3d vector3d::operator/ (double num){
    vector3d new_vec(this->x / num, this->y / num, this->z / num);
    return new_vec;
}

void vector3d::operator/= (double num){
    this->x /= num;
    this->y /= num;
    this->z /= num;
}


double vector3d::operator* (vector3d other){
    double dot = 0;
    dot += this->x * other.x;
    dot += this->y * other.y;
    dot += this->z * other.z;
    return dot;
}

vector3d vector3d::operator% (vector3d other){
    double x_ = (this->y * other.z) - (this->z * other.y);
    double y_ = (this->z * other.x) - (this->x * other.z);
    double z_ = (this->x * other.y) - (this->y * other.x);
    vector3d crossed(x_, y_, z_);
    return crossed;
}

bool vector3d::operator==(vector3d other){
    return x == other.x && y == other.y && z == other.z;
}

double vector3d::magnitude(){
    return sqrt( x*x + y*y + z*z );
}

vector3d vector3d::direction(){
    double mag = magnitude();
    if ( !mag ){ return vector3d(1e-9,0,0);}
    return (*this) / mag;
}

void vector3d::rotate(vector3d around, double angle){
    double mag = this->magnitude();
    if( around.magnitude() != 1){
        around = around.direction();
    }
    double co = cos(angle*deg_to_rad);
    double si = sin(angle*deg_to_rad);
    double ux = around.X();
    double uy = around.Y();
    double uz = around.Z();
    double new_x = x*( co + ux*ux*(1 - co) ) + y*( ux*uy*(1 - co) - uz*si ) + z*( ux*uz*( 1 - co ) + uy*si );
    double new_y = x*( ux*uy*(1 - co) + uz*si ) + y*( co + uy*uy*(1 - co) ) + z*( uy*uz*( 1 - co ) - ux*si );
    double new_z = x*( ux*uz*(1 - co) - uy*si ) + y*( uy*uz*( 1 - co ) + ux*si ) + z*( co + uz*uz*(1 - co) );
    new_x -= (new_x < ROTATION_ZERO_LIMIT && new_x > -ROTATION_ZERO_LIMIT) ? new_x : 0;
    new_y -= (new_y < ROTATION_ZERO_LIMIT && new_y > -ROTATION_ZERO_LIMIT) ? new_y : 0;
    new_z -= (new_z < ROTATION_ZERO_LIMIT && new_y > -ROTATION_ZERO_LIMIT) ? new_z : 0;
    x = new_x;
    y = new_y;
    z = new_z;
    *this *= mag / this->magnitude();
}



std::ostream& operator<<(std::ostream& stream, vector3d& vect){
    stream << std::setprecision(3) << "( " << vect.X() << " , " << vect.Y() << " , " << vect.Z() << " )";
    return stream;
}

std::ostream& operator<=(std::ostream& stream, vector3d& vect){
    stream << std::setprecision(4) << vect.X() << "   " << vect.Y() << "   " << vect.Z();
    return stream;
}


std::istream& operator>>(std::istream& stream, vector3d& vect){
    double x,y,z;
    stream >> "( " >> x >> " , " >> y >> " , " >> z >> " )";
    vect.setX(x); vect.setY(y); vect.setZ(z);
    return stream;
}

std::istream& operator>>(std::istream &is, char const *pat) {
    char ch;
    while (isspace(static_cast<unsigned char>(is.peek())))
        is.get(ch);
    while (*pat && is && *pat == is.peek() && is.get(ch)) {
        ++pat;
    }
    // if we didn't reach the end of the pattern, matching failed (mismatch, premature EOF, etc.)
    if (*pat) {
        is.setstate(std::ios::failbit);
    }
    return is;
}

#endif