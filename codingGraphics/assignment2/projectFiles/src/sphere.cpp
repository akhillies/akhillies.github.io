//
//  sphere.cpp
//  
//
//  Created by Akhil Batra on 3/13/14.
//
//

#include "sphere.h"
float sqr(float x) { return x*x;}

Sphere::Sphere() {
    radius = 0;
    center = Vec3(0, 0, 0);
}

Sphere::Sphere(float x, float y, float z, float r) {
    center = Vec3(x, y, z);
    radius = r;
}

float Sphere::getRadius() {
    return radius;
}

Vec3 Sphere::getCenter() {
    return center;
}

Vec3 Sphere::getNormal(Vec3 v) {
    return v.subVec3(getCenter()).normalize();
}

Vec3 Sphere::transformVec4(Vec3 v, float w) {
    return getTransformer().inverse().multiplyByVec4(v, w);
}

Vec3 Sphere::untransformVec4(Vec3 v, float w) {
    return getTransformer().multiplyByVec4(v, w);
}

Vec3 Sphere::calcDiffuse(Vec3 normal, Vec3 lightVec, Vec3 lightColor) {
    float diffConst = fmax(normal.dotProduct(lightVec), 0);
    return lightColor.multElements(getDiffuseRGB()).scale(diffConst);
}

Vec3 Sphere::calcSpec(Vec3 normal, Vec3 eyeVec, Vec3 lightVec, Vec3 lightColor) {
    Vec3 r = normal.scale(2 * normal.dotProduct(lightVec)).subVec3(lightVec).normalize();
    float sConst = pow(fmax(r.dotProduct(eyeVec), 0), getSpecIntensity());
    return lightColor.multElements(getSpecularRGB()).scale(sConst);
}

float Sphere::intersection(Ray r) {
    Vec3 rDir = r.getDirection();
    Vec3 oc = r.getSource().subVec3(transformVec4(getCenter(), 1));
    
    float inner = sqr(rDir.dotProduct(oc)) - rDir.dotProduct(rDir) * (oc.dotProduct(oc) - sqr(radius));
    if(inner < 0) {
        return -1;
    }
    
    float d1 = (-(rDir.dotProduct(oc)) - sqrt(inner)) / rDir.dotProduct(rDir);
    float d2 = (-(rDir.dotProduct(oc)) + sqrt(inner)) / rDir.dotProduct(rDir);
    if(r.inRange(d1)) {
        if(r.inRange(d2)) {
            return fmin(d1, d2);
        }
        return d1;
    }
    else if(r.inRange(d2)) {
        return d2;
    }
    return -1;
}

string Sphere::stringify() {
    ostringstream s;
    s << "Sphere with center at " << getCenter().stringify() << " and radius of " << getRadius();
    return s.str();
}