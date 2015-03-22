//
//  patch.cpp
//  
//
//  Created by Akhil Batra on 3/29/14.
//
//

#include "patch.h"

Patch::Patch() : Patch(Curve(), Curve(), Curve(), Curve()) {}

Patch::Patch(Curve first, Curve second, Curve third, Curve fourth)
{
    uCurves.push_back(Curve(first));
    uCurves.push_back(Curve(second));
    uCurves.push_back(Curve(third));
    uCurves.push_back(Curve(fourth));
}

Curve& Patch::operator[](int pos)
{
    if(pos < 4)
    {
        return uCurves[pos];
    }
    else
    {
        return vCurves[pos - 4];
    }
}

void Patch::assignVcurves()
{
    vCurves.push_back(Curve(uCurves[0][0], uCurves[1][0], uCurves[2][0], uCurves[3][0]));
    vCurves.push_back(Curve(uCurves[0][1], uCurves[1][1], uCurves[2][1], uCurves[3][1]));
    vCurves.push_back(Curve(uCurves[0][2], uCurves[1][2], uCurves[2][2], uCurves[3][2]));
    vCurves.push_back(Curve(uCurves[0][3], uCurves[1][3], uCurves[2][3], uCurves[3][3]));
}

pointNormalPair Patch::bezPatchInterpolation(float u, float v)
{
    Vec3 vPoint1 = uCurves[0].bezCurveInterpolation(u).point;
    Vec3 vPoint2 = uCurves[1].bezCurveInterpolation(u).point;
    Vec3 vPoint3 = uCurves[2].bezCurveInterpolation(u).point;
    Vec3 vPoint4 = uCurves[3].bezCurveInterpolation(u).point;
    
    Vec3 uPoint1 = vCurves[0].bezCurveInterpolation(v).point;
    Vec3 uPoint2 = vCurves[1].bezCurveInterpolation(v).point;
    Vec3 uPoint3 = vCurves[2].bezCurveInterpolation(v).point;
    Vec3 uPoint4 = vCurves[3].bezCurveInterpolation(v).point;
    
    Curve vCurv = Curve(vPoint1, vPoint2, vPoint3, vPoint4);
    Curve uCurv = Curve(uPoint1, uPoint2, uPoint3, uPoint4);
    
    pointDerivPair vResult = vCurv.bezCurveInterpolation(v);
    pointDerivPair uResult = uCurv.bezCurveInterpolation(u);
    
    Vec3 normal = uResult.derivative.cross(vResult.derivative).normalize();
    
    return pointNormalPair(uResult.point, normal);
}

string Patch::stringify()
{
    ostringstream s;
    s << uCurves[0].stringify() << "\n" << uCurves[1].stringify() << "\n" << uCurves[2].stringify() << "\n" << uCurves[3].stringify() << "\n" << vCurves[0].stringify() << "\n" << vCurves[1].stringify() << "\n" << vCurves[2].stringify() << "\n" << vCurves[3].stringify();
    return s.str();
}