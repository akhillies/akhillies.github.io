//
//  curve.h
//  
//
//  Created by Akhil Batra on 3/29/14.
//
//

#ifndef ____curve__
#define ____curve__

#include "vec3.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// struct that contains the pair of a point and the derivative along the curve as calculated by the Bezier interpolation method
struct pointDerivPair
{
    Vec3 point, derivative;
    pointDerivPair(Vec3 p, Vec3 d)
    {
        point = Vec3(p);
        derivative = Vec3(d);
    };
};

class Curve
{
private:
    // 4 control points for this Curve
    vector<Vec3> controlPts;
    
public:
    // default constructor for Curve, sets all vectors to 0,0,0
    Curve();
    
    // constructor for Curve that sets the 4 vectors to the given vectors
    Curve(Vec3 first, Vec3 second, Vec3 third, Vec3 fourth);
    
    // overloading the [] to get the Vec3 object asked by the index
    Vec3& operator[](int pos);
    
    // interpolates the curve at the given u and returns a point and its derivative along the curve
    pointDerivPair bezCurveInterpolation(float u);
    
    // converts this Curve into a string, used for testing purposes
    string stringify();
};

#endif /* defined(____curve__) */
