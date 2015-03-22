//
//  ray.h
//  
//
//  Created by Akhil Batra on 3/8/14.
//
//

#ifndef ____ray__
#define ____ray__

#include "vec3.h"

#include <string>
#include <sstream>

class Ray {
    private:
        // the point this ray originates from
        Vec3 sourcePoint;
    
        // the direction of this ray, should be a normalized vector
        Vec3 directVector;
    
        // min of t
        float tMin;
    
        // max of t
        float tMax;
    
    public:
        // constructor for the ray, takes in all the necessary parameters
        Ray(Vec3 start, Vec3 direct, float mn, float mx);
    
        // checks to see if the given float is in the range of this ray's t
        bool inRange(float t);
    
        // gets the tmin
        float getMinT();
    
        // gets the tmax
        float getMaxT();
    
        // returns the point reached after applying tMax steps
        Vec3 applySteps(float t);

        // returns the source point
        Vec3 getSource();

        // returns the direction vector
        Vec3 getDirection();
    
        // stringify the ray for testing / debugging purposes
        string stringify();
};

#endif /* defined(____ray__) */