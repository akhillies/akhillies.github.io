//
//  vec3.cpp
//  
//
//  Created by Akhil Batra on 3/8/14.
//
//


#include "vec3.h"


Vec3::Vec3() : Vec3(0, 0, 0) {}

Vec3::Vec3(float i, float j, float k)
{
    setX(i);
    setY(j);
    setZ(k);
}

float Vec3::getX()
{
    return x;
}

float Vec3::getY()
{
    return y;
}

float Vec3::getZ()
{
    return z;
}

void Vec3::setX(float i)
{
    x = i;
}

void Vec3::setY(float j)
{
    y = j;
}

void Vec3::setZ(float k)
{
    z = k;
}

void Vec3::setXYZ(float i, float j, float k)
{
    setX(i);
    setY(j);
    setZ(k);
}

Vec3 Vec3::operator+(const Vec3& v)
{
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3& v)
{
    return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator*(const Vec3& v)
{
    return Vec3(x * v.x, y * v.y, z * v.z);
}

Vec3 Vec3::operator/(const Vec3& v)
{
    return Vec3(x / v.x, y / v.y, z / v.z);
}

Vec3 Vec3::scale(float s) {
    return Vec3(getX() * s, getY() * s, getZ() * s);
}

Vec3 Vec3::normalize()
{
    return scale(1.0 / magnitude());
}

string Vec3::stringify()
{
    ostringstream s;
    s << "<" << getX() << " " << getY() << " " << getZ() << ">";
    return s.str();
}

float Vec3::distance(Vec3 v)
{
    Vec3 ths = *this;
    return sqrt(((ths - v) * (ths - v)).sumElements());
}

float Vec3::magnitude()
{
    Vec3 ths = *this;
    return sqrt((ths * ths).sumElements());
}

float Vec3::dot(Vec3 v)
{
    Vec3 ths = *this;
    return (ths * v).sumElements();
}

Vec3 Vec3::cross(Vec3 v)
{
    return Vec3(getY() * v.getZ() - getZ() * v.getY(), v.getX() * getZ() - v.getZ() * getX(), getX() * v.getY() - getY() * v.getX());
}

float Vec3::sumElements()
{
    return getX() + getY() + getZ();
}

Vec3 Vec3::midpoint(Vec3 v)
{
    Vec3 ths = *this;
    return (ths + v).scale(0.5f);
}

bool Vec3::operator==(const Vec3& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

bool Vec3::operator!=(const Vec3& v) const
{
    return !operator==(v);
}

bool Vec3::operator< (const Vec3& v) const
{
    if(x == v.x)
    {
        if(y == v.y)
        {
            return z < v.z;
        }
        return y < v.y;
    }
    return x < v.x;
}

bool Vec3::operator> (const Vec3& v) const
{
    if(x == v.x)
    {
        if(y == v.y)
        {
            return z > v.z;
        }
        return y > v.y;
    }
    return x > v.x;
}

bool Vec3::operator<=(const Vec3& v) const
{
    return !operator> (v);
}

bool Vec3::operator>=(const Vec3& v) const
{
    return !operator< (v);
}


