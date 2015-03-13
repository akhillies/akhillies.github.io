//
//  Triangle.h
//
//
//  Created by Akhil Batra on 4/15/14.
//
//

#ifndef ____Triangle__
#define ____Triangle__

#include "vec3.h"
#include "patch.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>


struct Vertex
{
    Vec3 point;
    Vec3 normal;
    float u;
    float v;
};

struct Edges
{
    bool oneTwo;
    bool twoThree;
    bool threeOne;
    Vertex mid12;
    Vertex mid23;
    Vertex mid31;
};

class Triangle
{
private:
    vector<Vertex> vertices;
    Patch *surface;
    
public:
    // default constructor, sets everything to 0s all around
    Triangle();
    
    // takes the given patch and vertices and saves them
    Triangle(Patch *p, Vertex p1, Vertex p2, Vertex p3);
    
    // overloading [] for easy access of Vertices
    Vertex& operator[](int pos);
    
    // getter for this triangle's patch
    Patch* getPatch();
    
    // checks to see if this triangle is flat, and returns a struct that contains bools saying which edge is/isn't flat as well as all the midpoints
    Edges checkFlatness(float err);
    
    // way to display the triangle!!
    string stringify();
};

#endif /* defined(____Triangle__) */
