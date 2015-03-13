//
//  curve.cpp
//  
//
//  Created by Akhil Batra on 3/29/14.
//
//

#include "curve.h"

Curve::Curve() : Curve(Vec3(), Vec3(), Vec3(), Vec3()) {}

Curve::Curve(Vec3 first, Vec3 second, Vec3 third, Vec3 fourth)
{
    controlPts.push_back(Vec3(first));
    controlPts.push_back(Vec3(second));
    controlPts.push_back(Vec3(third));
    controlPts.push_back(Vec3(fourth));
}

Vec3& Curve::operator[](int pos)
{
    return controlPts[pos];
}

pointDerivPair Curve::bezCurveInterpolation(float u)
{
    Curve ths = *this;

    Vec3 aPt = ths[0].scale(1 - u) + ths[1].scale(u);
    Vec3 bPt = ths[1].scale(1 - u) + ths[2].scale(u);
    Vec3 cPt = ths[2].scale(1 - u) + ths[3].scale(u);
    
    Vec3 dPt = aPt.scale(1 - u) + bPt.scale(u);
    Vec3 ePt = bPt.scale(1 - u) + cPt.scale(u);
    
    Vec3 point = dPt.scale(1 - u) + ePt.scale(u);
    Vec3 deriv = (ePt - dPt).scale(3);

    return pointDerivPair(point, deriv);
}

string Curve::stringify()
{
    ostringstream s;
    Curve ths = *this;
    s << "{" << ths[0].stringify() << " " << ths[1].stringify() << " " << ths[2].stringify() << " " << ths[3].stringify() << "}";
    return s.str();
}