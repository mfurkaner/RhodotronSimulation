#include "vector.h"
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

double vector3d::magnitude(){
    return sqrt( x*x + y*y + z*z );
}

vector3d vector3d::direction(){
    double mag = magnitude();
    return (*this) / mag;
}



std::ostream& operator<<(std::ostream& stream, vector3d& vect){
    stream << std::setprecision(3) << "( " << vect.X() << ", " << vect.Y() << ", " << vect.Z() << " )";
    return stream;
}