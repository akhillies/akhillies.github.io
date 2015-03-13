//
//  system.h
//  
//
//  Created by Akhil Batra on 5/7/14.
//
//

#ifndef ____system__
#define ____system__

#include "joint.h"
#include "structs.h"
#include "../Eigen/Dense"
#include <iostream>
#include <vector>

using namespace::std;
using namespace::Eigen;

class System
{
private:
    // list of the joints in the system
    vector<Joint> joints;
    
    // minimum distance an endEffector is allowed to be from the goal
    float error;
    
    // maximum length this system can extend to
    float length;
    
    // end effector of the system
    Vector3f endEffector;
    
public:
    // default constructor
    System();
    
    // constructor that takes an error value
    System(float err);
    
    // creates the Jacobian of all the Joints in this system
    Matrix3Xf createJacobian();
    
    // updates all the end effectors and rotations of each joint in the system
    void update(VectorXf dr);
    
    // gets the number of joints currently in the System
    int numJoints();
    
    // returns the length of the whole system
    float getLength();
    
    // getter for error
    float getError();
    
    // returns the end effector of this system
    Vector3f getEndEffector();
    
    // setter for error
    void setError(float err);
    
    // adds the given joint to the end of the list
    void addJoint(Joint j);
    
    // draws all the joints in this system
    void drawJoints();
    
    // creates an identity matrix of size n x n 
    MatrixXf identityMatrix(int n);
    
    // operator overload of [] to get the i-th joint in the system
    Joint& operator[](int i);
    
    // solves the IK system to get to the next goal, ???returns the list of changes in rotation so the drawer can implement those small changes to make the movement smoother???
    // returns bool for whether the joints were actually moved or not
    bool applyIK(Vector3f goal);
};

#endif /* defined(____system__) */
