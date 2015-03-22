//
//  camera.h
//  
//
//  Created by Akhil Batra on 3/8/14.
//
//

#ifndef ____camera__
#define ____camera__

#include "ray.h"

#include <iostream>
#include <math.h>

#endif /* defined(____camera__) */

class Camera {
private:
    // the starting point of the camera
    Vec3 origin;
    
    // the number of pixels in height and width
    float height, width;
    
    // four corners of image plane
    Vec3 lowerLeft, lowerRight, upperLeft, upperRight;
    
public:
    // constructor, takes in the point, and the width/height of screen and defaults the camera position to (0, 0, 1)
    Camera(float nx, float ny, Vec3 ll, Vec3 lr, Vec3 ul, Vec3 ur);
    
    // constructor, takes in the point where the camera will be placed and the width/height of screen
    Camera(Vec3 e, float nx, float ny, Vec3 ll, Vec3 lr, Vec3 ul, Vec3 ur);
    
    // generates a ray from the camera to the given pixel
    Ray generateViewingRay(float i, float j, float tmn=1, float tmx=250);
};