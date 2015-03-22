//
//  vec3.cpp
//  
//
//  Created by Akhil Batra on 3/8/14.
//
//


#include "vec3.h"


Vec3::Vec3() {
    setX(0);
    setY(0);
    setZ(0);
}

Vec3::Vec3(float i, float j, float k) {
    setX(i);
    setY(j);
    setZ(k);
}

float Vec3::getX() {
    return x;
}

float Vec3::getY() {
    return y;
}

float Vec3::getZ() {
    return z;
}

void Vec3::setX(float i) {
    x = i;
}

void Vec3::setY(float j) {
    y = j;
}

void Vec3::setZ(float k) {
    z = k;
}

void Vec3::setXYZ(float i, float j, float k) {
    setX(i);
    setY(j);
    setZ(k);
}

Vec3 Vec3::addVec3(Vec3 v) {
    return Vec3(getX() + v.getX(), getY() + v.getY(), getZ() + v.getZ());
}

Vec3 Vec3::subVec3(Vec3 v) {
    return Vec3(getX() - v.getX(), getY() - v.getY(), getZ() - v.getZ());
}

Vec3 Vec3::multElements(Vec3 v) {
    return Vec3(getX() * v.getX(), getY() * v.getY(), getZ() * v.getZ());
}

Vec3 Vec3::divElements(Vec3 v) {
    return Vec3(getX() / v.getX(), getY() / v.getY(), getZ() / v.getZ());
}

Vec3 Vec3::scale(float s) {
    return Vec3(getX() * s, getY() * s, getZ() * s);
}

Vec3 Vec3::normalize() {
    return scale(1.0 / magnitude());
}

string Vec3::stringify() {
    ostringstream s;
    s << "{" << getX() << " " << getY() << " " << getZ() << "}";
    return s.str();
}

float Vec3::distance(Vec3 v) {
    return sqrt(pow(getX() - v.getX(), 2) + pow(getY() - v.getY(), 2) + pow(getZ() - v.getZ(), 2));
}

float Vec3::magnitude() {
    return sqrt(pow(getX(), 2) + pow(getY(), 2) + pow(getZ(), 2));
}

float Vec3::dotProduct(Vec3 v) {
    return getX() * v.getX() + getY() * v.getY() + getZ() * v.getZ();
}

Vec3 Vec3::crossProduct(Vec3 v) {
    return Vec3(getY() * v.getZ() - getZ() * v.getY(), v.getX() * getZ() - v.getZ() * getX(), getX() * v.getY() - getY() * v.getX());
}