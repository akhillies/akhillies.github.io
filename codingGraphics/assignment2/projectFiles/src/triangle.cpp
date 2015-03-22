//
//  triangle.cpp
//  
//
//  Created by Akhil Batra on 3/9/14.
//
//

#include "triangle.h"

Triangle::Triangle() {
    one = Vec3();
    two = Vec3();
    three = Vec3();
}

Triangle::Triangle(Vec3 f, Vec3 s, Vec3 t) {
    one = f;
    two = s;
    three = t;
}

Vec3 Triangle::transformVec4(Vec3 v, float w) {
    return v;
}

Vec3 Triangle::untransformVec4(Vec3 v, float w) {
    return v;
}

Vec3 Triangle::calcDiffuse(Vec3 normal, Vec3 lightVec, Vec3 lightColor) {
    float diffConst = fmax(normal.dotProduct(lightVec), 0);
    return lightColor.multElements(getDiffuseRGB()).scale(diffConst);
}

Vec3 Triangle::calcSpec(Vec3 normal, Vec3 eyeVec, Vec3 lightVec, Vec3 lightColor) {
    Vec3 r = normal.scale(2 * normal.dotProduct(lightVec)).subVec3(lightVec).normalize();
    float sConst = pow(fmax(r.dotProduct(eyeVec), 0), getSpecIntensity());
    return lightColor.multElements(getSpecularRGB()).scale(sConst);
}

Vec3 Triangle::getFirst() {
    return one;
}

Vec3 Triangle::getSecond() {
    return two;
}

Vec3 Triangle::getThird() {
    return three;
}

Vec3 Triangle::getNormal(Vec3 v) {
    return getSecond().subVec3(getFirst()).crossProduct(getThird().subVec3(getFirst())).normalize();
}

string Triangle::stringify() {
    
    ostringstream s;
    s << "Triangle with points at " << getFirst().stringify() << ", " << getSecond().stringify() << ", " << getThird().stringify();
    return s.str();
}

float Triangle::intersection(Ray r) {
    float a = getFirst().getX() - getSecond().getX();
    float d = getFirst().getX() - getThird().getX();
    float g = r.getDirection().getX();
    float b = getFirst().getY() - getSecond().getY();
    float e = getFirst().getY() - getThird().getY();
    float h = r.getDirection().getY();
    float c = getFirst().getZ() - getSecond().getZ();
    float f = getFirst().getZ() - getThird().getZ();
    float i = r.getDirection().getZ();
    float j = getFirst().getX() - r.getSource().getX();
    float k = getFirst().getY() - r.getSource().getY();
    float l = getFirst().getZ() - r.getSource().getZ();
    
    float m = a * (e * i - h * f) + b * (g * f - d * i) + c * (d * h - e * g);
    
    float beta = (j * ( e * i - h * f) + k * (g * f - d * i) + l * (d * h - e * g)) / m;
    float gamma = (i * (a * k - j * b) + h * (j * c - a * l) + g * (b * l - k * c)) / m;
    float t = -(f * (a * k - j * b) + e * (j * c - a * l) + d * (b * l - k * c)) / m;
    
    if(gamma >= 0 && gamma <= 1 && beta >= 0 && beta <= 1 - gamma) {
        if(r.inRange(t)) {
            return t;
        }
    }
    return -1;
}