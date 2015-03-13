//
//  Joint.h
//  
//
//  Created by Akhil Batra on 5/7/14.
//
//

#ifndef ____joint__
#define ____joint__

#include "../Eigen/Dense"
#include <iostream>
#include <string>
#include <GLUT/glut.h>
#include <OpenGL/glu.h>

using namespace::std;
using namespace::Eigen;

class Joint
{
private:
    // length of this joint
    float length;
    
    // origin of this point
//    Vector3f origin;
    
    // end point of this joint IN LOCAL SPACE - basically just a container for other classes to use, this is never used by the Joint class
    Vector3f endPoint;
    
    // the exponential map representing the rotation applied to this joint FROM THE ORIGINAL AXIS (not the previous joint) - (x, y, z) represents the axis of rotation and the magnitude represents the degrees of rotation
    Vector3f rotation;

public:
    // default constructor, will set everything to 0
    Joint();
    
    // constructor for this joint that takes everything
//    Joint(float l, Vector3f o, Vector3f r);
    Joint(float l, Vector3f r);
    
    // creates the Jacobian for this joint
//    Matrix3f getJacobian(Vector3f endEffector);
    Matrix3f getJacobian();
    
    // updates the end effector and the rotation by applying the given new origin and change in rotation
//    void update(Vector3f o, Vector3f dr);
    void update(Vector3f dr);
    
    // getter for the length of this joint
    float getLength();
    
    // setter for the length of this joint
    void setLength(float l);
    
    // getter for the origin
//    Vector3f getOrigin();
    
    // getter for the end point of this joint relative to the previous joint
    Vector3f getEndPoint();
    
    // getter for the exponential map of this joint relative to the previous joint
    Vector3f getExponentialMap();
    
    // getter for the rotation matrix, derived from the exponential map
    Matrix3f getRotationMatrix();
    
    // sets the end point of this joint
//    void setEndPoint(Vector3f newE);
    
    // applys the Rodrigues' rotation formula to the given v with the exponential map
    Vector3f rodriguesRotate(Vector3f v);
    
    // draws the shape that is this joint :D includes the cone for the joint and a sphere for the ball base
//    void draw(Vector3f origin);
    void draw();
    
    // converts this Joint into a string that can be read and used for testing purposes
    string stringify();
};

#endif /* defined(____joint__) */
