//
//  ray.cpp
//  
//
//  Created by Akhil Batra on 3/8/14.
//
//

#include "ray.h"

Ray::Ray(Vec3 start, Vec3 direct, float mn, float mx) {
    sourcePoint = start;
    directVector = direct;
    tMax = mx;
    tMin = mn;
}

bool Ray::inRange(float t) {
    return t >= tMin && t <= tMax;
}

float Ray::getMinT() {
    return tMin;
}

float Ray::getMaxT() {
    return tMax;
}

Vec3 Ray::applySteps(float t) {
    return getSource().addVec3(getDirection().scale(t));
}

Vec3 Ray::getSource() {
    return sourcePoint;
}

Vec3 Ray::getDirection() {
    return directVector;
}

string Ray::stringify() {
    ostringstream s;
    s << "Ray is " << getSource().stringify() << " towards " << getDirection().stringify();
    return s.str();
}