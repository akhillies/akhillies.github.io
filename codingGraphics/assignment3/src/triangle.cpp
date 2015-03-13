//
//  triangle.cpp
//  
//
//  Created by Akhil Batra on 4/15/14.
//
//

#include "triangle.h"


Triangle::Triangle() : Triangle(nullptr, Vertex(), Vertex(), Vertex()) {}

Triangle::Triangle(Patch *p, Vertex p1, Vertex p2, Vertex p3)
{
    surface = p;
    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
}

Vertex& Triangle::operator[](int pos)
{
    return vertices[pos];
}

Patch* Triangle::getPatch()
{
    return surface;
}

Edges Triangle::checkFlatness(float err)
{
    Triangle ths = *this;
    Edges e;
    
    e.mid12.u = (ths[0].u + ths[1].u) / 2;
    e.mid12.v = (ths[0].v + ths[1].v) / 2;
    
    e.mid23.u = (ths[1].u + ths[2].u) / 2;
    e.mid23.v = (ths[1].v + ths[2].v) / 2;
    
    e.mid31.u = (ths[2].u + ths[0].u) / 2;
    e.mid31.v = (ths[2].v + ths[0].v) / 2;
    
    pointNormalPair pnp12 = surface->bezPatchInterpolation(e.mid12.u, e.mid12.v);
    pointNormalPair pnp23 = surface->bezPatchInterpolation(e.mid23.u, e.mid23.v);
    pointNormalPair pnp31 = surface->bezPatchInterpolation(e.mid31.u, e.mid31.v);
    
    float dist12 = ths[0].point.midpoint(ths[1].point).distance(pnp12.point);
    float dist23 = ths[1].point.midpoint(ths[2].point).distance(pnp23.point);
    float dist31 = ths[2].point.midpoint(ths[0].point).distance(pnp31.point);
    
    e.oneTwo = (dist12 < err);
    e.twoThree = (dist23 < err);
    e.threeOne = (dist31 < err);
    
    e.mid12.point = Vec3(pnp12.point);
    e.mid23.point = Vec3(pnp23.point);
    e.mid31.point = Vec3(pnp31.point);
    
    e.mid12.normal = Vec3(pnp12.normal);
    e.mid23.normal = Vec3(pnp23.normal);
    e.mid31.normal = Vec3(pnp31.normal);
    
    return e;
}


string Triangle::stringify()
{
    ostringstream s;
    s << "{" << vertices[0].point.stringify() << " " << vertices[1].point.stringify() << " " << vertices[2].point.stringify() << "}";
    return s.str();
}