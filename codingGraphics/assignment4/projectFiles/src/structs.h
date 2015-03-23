//
//  structs.cpp
//  
//
//  Created by Akhil Batra on 5/5/14.
//
//

#ifndef ____structs__
#define ____structs__

#include <vector>
#include "../Eigen/Dense"

using namespace::std;
using namespace::Eigen;

struct JointRotations
{
    // set of rotations at each timestep (so at time 0, the rotations for all the joints are at index 0)
    vector<MatrixXf> rotates;
};

struct Viewport
{
    int w;
    int h;
};

#endif /* defined(____structs__) */