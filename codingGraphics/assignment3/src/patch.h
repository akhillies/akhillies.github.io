//
//  patch.h
//  
//
//  Created by Akhil Batra on 3/29/14.
//
//

#ifndef ____patch__
#define ____patch__

#include "curve.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

struct pointNormalPair
{
    Vec3 point, normal;
    pointNormalPair(Vec3 p, Vec3 n)
    {
        point = Vec3(p);
        normal = Vec3(n);
    }
};

class Patch
{
private:
    // the 4 curves in u direction for this patch
    vector<Curve> uCurves;
    
    // the 4 curves in v direction for this patch
    vector<Curve> vCurves;
    
    
public:
    // default constructor for patch, sets all Curve to the default curve
    Patch();
    
    // constructor for Patch that sets the 4 u curves to the given Curves and from there sets the 4 v curves
    Patch(Curve first, Curve second, Curve third, Curve fourth);
    
    // overloading the [] to get the Vec3 object asked by the index
    Curve& operator[](int pos);
    
    // builds the control points for the Bezier curve in the given float then finds the surface point and its normal and returns them
    pointNormalPair bezPatchInterpolation(float u, float v);
    
//    // checks if the given point is in this patch
//    bool inPatch(Vec3 v);
    
    // creates the vCurves of this patch - should be called right after all the uCurves have been assigned
    void assignVcurves();
    
    // converts Patch to string
    string stringify();
};

#endif /* defined(____patch__) */
