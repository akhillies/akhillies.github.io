//
//  camera.cpp
//  
//
//  Created by Akhil Batra on 3/8/14.
//
//

#include "camera.h"

Camera::Camera(float nx, float ny, Vec3 ll, Vec3 lr, Vec3 ul, Vec3 ur) {
    width = nx;
    height = ny;
    lowerLeft = Vec3(ll.getX(), ll.getY(), ll.getZ());
    lowerRight = Vec3(lr.getX(), lr.getY(), lr.getZ());
    upperLeft = Vec3(ul.getX(), ul.getY(), ul.getZ());
    upperRight = Vec3(ur.getX(), ur.getY(), ur.getZ());
    origin = Vec3(0, 0, 0);
}

Camera::Camera(Vec3 o, float nx, float ny, Vec3 ll, Vec3 lr, Vec3 ul, Vec3 ur) {
    origin = Vec3(o.getX(), o.getY(),o.getZ());
    width = nx;
    height = ny;
    lowerLeft = Vec3(ll.getX(), ll.getY(), ll.getZ());
    lowerRight = Vec3(lr.getX(), lr.getY(), lr.getZ());
    upperLeft = Vec3(ul.getX(), ul.getY(), ul.getZ());
    upperRight = Vec3(ur.getX(), ur.getY(), ur.getZ());
}

Ray Camera::generateViewingRay(float i, float j, float tmn, float tmx) {
    float u = (i + 0.5) / width;
    float v = (j + 0.5) / height;
    Vec3 point = lowerLeft.scale(1 - v).addVec3(upperLeft.scale(v)).scale(1 - u).addVec3(lowerRight.scale(1 - v).addVec3(upperRight.scale(v)).scale(u));
    return Ray(origin, point.subVec3(origin), tmn, tmx);
}