//
//  triangle.h
//  
//
//  Created by Akhil Batra on 3/9/14.
//
//

#ifndef ____triangle__
#define ____triangle__

#include "ray.h"
#include "matrix.h"
#include "shape.h"

#include <vector>
#include <string>
#include <sstream>

#endif /* defined(____triangle__) */

class Triangle : public Shape {
private:
    // the 3 vertices of this triangle
    Vec3 one, two, three;
    
public:
    // simple constructor, just sets all 3 vertices to 0,0,0
    Triangle();
    
    // constructor that should be used in all situations, sets all 3 vertices to given ones
    Triangle(Vec3 f, Vec3 s, Vec3 t);
    
    // gets the "first" vertex, can be anything really
    Vec3 getFirst();
    
    // gets the "second" vertex, should be counter-clockwise to the first vertex
    Vec3 getSecond();
    
    // gets the "third" vertex, should be counter-clockwise to the second vertex
    Vec3 getThird();
    
    // converts the triangle to a string - basically just a list of its vertices
    string stringify();
    
    // calculates the rgb coming from the diffuse material with the given light
    Vec3 calcDiffuse(Vec3 normal, Vec3 lightPt, Vec3 lightColor);
    
    // calculates the rgb coming from the specular material with the given light
    Vec3 calcSpec(Vec3 normal, Vec3 eyeVec, Vec3 lightVec, Vec3 lightColor);
    
    // calculates how many steps it takes for the given ray to intersection with this triangle, or 0 if no intersection
    float intersection(Ray r);
    
    // returns the normal of the plane
    Vec3 getNormal(Vec3 v);
    
    Vec3 transformVec4(Vec3 v, float w);
    
    Vec3 untransformVec4(Vec3 v, float w);
};