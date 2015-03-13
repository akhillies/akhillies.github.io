//
//  sphere.h
//  
//
//  Created by Akhil Batra on 3/13/14.
//
//

#ifndef ____sphere__
#define ____sphere__

#include "ray.h"
#include "shape.h"
#include <iostream>
#include <string>
#include <sstream>

class Sphere : public Shape {
private:
    float radius;
    Vec3 center;
    
public:
    // default sphere constructor, sets radius to 0 and center to 0, 0, 0
    Sphere();
    
    // sets this sphere object to have the given center and radius
    Sphere(float x, float y, float z, float r);
    
    // getter for radius
    float getRadius();
    
    // getter for the center point
    Vec3 getCenter();
    
    // finds the number of t steps it takes for the give ray to intersect with this radius or -1 if there is no intersection
    float intersection(Ray r);
    
    // converts the sphere to a string to be read for testing purposes
    string stringify();
    
    // returns the normal of the given point on this shape
    Vec3 getNormal(Vec3 v);
    
    // calculates the rgb coming from the diffuse material with the given light
    Vec3 calcDiffuse(Vec3 normal, Vec3 lightVec, Vec3 lightColor);
    
    // calculates the rgb coming from the specular material with the given light
    Vec3 calcSpec(Vec3 normal, Vec3 eyeVec, Vec3 lightVec, Vec3 lightColor);
    
    // transforms the given vector into object space from world space
    Vec3 transformVec4(Vec3 v, float w);
    
    // untransforms the given vector from this object space to the world space
    Vec3 untransformVec4(Vec3 v, float w);
};

#endif /* defined(____sphere__) */
