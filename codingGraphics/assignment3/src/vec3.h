//
//  vec3.h
//
//
//  Created by Akhil Batra on 3/8/14.
//
//

#ifndef ____vec3__
#define ____vec3__

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>


using namespace std;

class Vec3
{
    
private:
    // xyz elemnts of this vector
    float x, y, z;
    
    
public:
    // default constructor, sets everything to 0
    Vec3();
    
    // constructor that sets everything to the given floats
    Vec3(float i, float j, float k);
    
    // returns the x element
    float getX();
    
    // returns the y element
    float getY();
    
    // returns the z element
    float getZ();
    
    // sets the x element to the given float
    void setX(float i);
    
    // sets the y element to the given float
    void setY(float j);
    
    // sets the z element to the given float
    void setZ(float k);
    
    // sets all the elements to the given 3 floats
    void setXYZ(float i, float j, float k);
    
    // adds the given vector to this one and returns a new vector (overloads +operator)
    Vec3 operator+(const Vec3& v);
    
    // subtracts the given vector from this vector and returns a new vector (overloads -operator)
    Vec3 operator-(const Vec3& v);
    
    // multiplies all the elements in this vector by each of the elements in the given vector (the x elements multiplied by each other, etc.) (overloads *operator)
    Vec3 operator*(const Vec3& v);
    
    // divides all the elements in this vector by each of the elements in the given vector (the x elements divided by each other, etc.) (overloads /operator)
    Vec3 operator/(const Vec3& v);
    
    // overloading comparators for Vec3 objects - does comparasions by looking at the xyz coordinates and compares them (first compares x and if equal, then y and if also equal, then z)
    bool operator<(const Vec3& v) const;
    bool operator<=(const Vec3& v) const;
    bool operator>(const Vec3& v) const;
    bool operator>=(const Vec3& v) const;
    bool operator==(const Vec3& v) const;
    bool operator!=(const Vec3& v) const;
    
    // scales all the elements of this vector by the given float
    Vec3 scale(float s);
    
    // returns the vector that is the normalized version of this vector
    Vec3 normalize();
    
    // returns a string with the information of this vector
    string stringify();
    
    // gives the result of the distance formula between this vector and the given vector
    float distance(Vec3 v);
    
    // returns the magnitude of this vector
    float magnitude();
    
    // returns the dot product of this vector and the given vector
    float dot(Vec3 v);
    
    // returns a new vector that is cross product between this vector and the given vector
    Vec3 cross(Vec3 v);
    
    // returns the sum of all the elements in this vector
    float sumElements();
    
    // returns the midpoint of this vector and the given vector
    Vec3 midpoint(Vec3 v);
};

#endif /* defined(____vec3__) */