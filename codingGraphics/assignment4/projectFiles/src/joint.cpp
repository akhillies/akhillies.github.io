//
//  Joint.cpp
//  
//
//  Created by Akhil Batra on 5/7/14.
//
//

#include "joint.h"

//Joint::Joint():Joint(0, Vector3f(), Vector3f()){}
Joint::Joint():Joint(0, Vector3f(0, 0, 0)){}

//Joint::Joint(float l, Vector3f o, Vector3f r)
Joint::Joint(float l, Vector3f r)
{
    length = l;
//    origin = Vector3f(o);
    rotation = Vector3f(0, 0, 0);
    endPoint = Vector3f(0, 0, length);
    
//    update(origin, r);
    update(r);
}

//Matrix3f Joint::getJacobian(Vector3f endEffector)
Matrix3f Joint::getJacobian()
{
//    Vector3f de = endEffector - endPoint;
    Vector3f de = endPoint;
    Matrix3f J;
    J(0, 0) = 0;
    J(0, 1) = -de(2);
    J(0, 2) = de(1);
    J(1, 0) = de(2);
    J(1, 1) = 0;
    J(1, 2) = -de(0);
    J(2, 0) = -de(1);
    J(2, 1) = de(0);
    J(2, 2) = 0;
    return -J;
}

//void Joint::update(Vector3f o, Vector3f dr)
void Joint::update(Vector3f dr)
{
//    origin = o;
//    endPoint = Vector3f(0, 0, length);
    if(dr(0) != dr(0) || dr(1) != dr(1) || dr(2) != dr(2))
    {
        cout << "dr was NAN" << endl;
        return;
    }
//    cout << "sdf" << endl;
//    cout << "when updating joint" << endl << dr << endl << endl;
    rotation += dr;
    endPoint = rodriguesRotate(Vector3f(0, 0, length));
//    if(endPoint(2) < 0)
//    {
//        Vector3f rotNorm = rotation.normalized();
//        Vector3f point(0, 0, length);
//        float a = point(2);
//        float b = rotNorm.cross(point)(2);
//        float c = (rotNorm * rotNorm.dot(point))(2);
//        float theta = 2 * (3.1415962 - atan(.5 * a / b));
//        cout << b << endl;
//        rotation = rotation.normalized() * theta;
//        endPoint = rodriguesRotate(Vector3f(0, 0, length));
//    }

//    cout << this->stringify() << endl << endl << endl;
    
//    float theta = rotation.norm();
//    
//    if(theta != 0)
//    {
//        Vector3f axis = rotation.normalized();
//        
//        endPoint = endPoint * cos(theta) + (axis.cross(endPoint)) * sin(theta) + axis * (axis.dot(endPoint)) * (1 - cos(theta));
//    }
//    endPoint += origin;
}

float Joint::getLength()
{
    return length;
}

void Joint::setLength(float l)
{
    length = l;
}

//Vector3f Joint::getOrigin()
//{
//    return origin;
//}

Vector3f Joint::getEndPoint()
{
    return endPoint;
}

Vector3f Joint::getExponentialMap()
{
    return rotation;
}

Matrix3f Joint::getRotationMatrix()
{
    Matrix3f i;
    i <<    1, 0, 0,
            0, 1, 0,
            0, 0, 1;
    
    float theta = rotation.norm();
    if(theta == 0)
    {
        return i;
    }
    
    Vector3f axis = rotation.normalized();
    Matrix3f rot;
    rot(0, 0) = 0;
    rot(0, 1) = -axis(2);
    rot(0, 2) = axis(1);
    rot(1, 0) = axis(2);
    rot(1, 1) = 0;
    rot(1, 2) = -axis(0);
    rot(2, 0) = -axis(1);
    rot(2, 1) = axis(0);
    rot(2, 2) = 0;
    
//    Matrix3f ro;
//    ro(0, 0) = axis(0) * axis(0);
//    ro(0, 1) = axis(0) * axis(1);
//    ro(0, 2) = axis(0) * axis(2);
//    ro(1, 0) = axis(1) * axis(0);
//    ro(1, 1) = axis(1) * axis(1);
//    ro(1, 2) = axis(1) * axis(2);
//    ro(2, 0) = axis(2) * axis(0);
//    ro(2, 1) = axis(2) * axis(1);
//    ro(2, 2) = axis(2) * axis(2);
    
//    Matrix3f r = cos(theta) * i + sin(theta) * rot + (1 - cos(theta)) * ro;
    Matrix3f r = i + sin(theta) * rot + (1 - cos(theta)) * (rot * rot);//axis * axis.transpose() + sin(theta) * rot - cos(theta) * (rot * rot);
//    cout << "processing" << endl;
//    if(r.determinant() != 1)
//    {
//        cout << "rotation determinant" << endl;
//    }
//    if(r.inverse() != r.transpose())
//    {
//        cout << "transpose different from inverse" << endl;
//        cout << r << endl;
//    }
//    cout << endl;
    return r;
//    return i + sin(theta) * rot + (1 - cos(theta)) * (rot * rot);
}

//void Joint::setEndPoint(Vector3f newE)
//{
//    endPoint = Vector3f(newE);
//}

Vector3f Joint::rodriguesRotate(Vector3f v)
{
    float theta = rotation.norm();
    if(theta != 0)
    {
        Vector3f axis = rotation.normalized();
        //        return v * cos(theta) + (axis.cross(v)) * sin(theta) + axis * (axis.dot(v)) * (1 - cos(theta));
        return axis * axis.dot(v) + axis.cross(v) * sin(theta) - axis.cross(axis.cross(v)) * cos(theta);
    }
    return v;
}

//void Joint::draw(Vector3f origin)
void Joint::draw()
{
    glPushMatrix();
    
//    glTranslatef(origin(0), origin(1), origin(2));
    glRotatef(rotation.norm() / 3.14159265359 * 180, rotation(0), rotation(1), rotation(2));

    GLUquadric* quad = gluNewQuadric();
    
    gluQuadricNormals(quad, GL_SMOOTH);
    gluQuadricDrawStyle(quad, GLU_FILL);
    
    float c[] = {0, .71, 1, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, c);
    gluCylinder(quad, fmin(length / 4.0, .75), 0, length, 32, 32);
    glPushMatrix();
    glScalef(.9, .9, 1);
    gluSphere(quad, fmin(length / 4.0, .75), 32, 32);
    glPopMatrix();
    
//    glPopMatrix();
    glTranslatef(0, 0, length);
}

string Joint::stringify()
{
    stringstream out;
    out << "Joint of length " << length << " with the end point at (" << endPoint(0) << ", " << endPoint(1) << ", " << endPoint(2) << ") and a rotation vector of (" << rotation(0) << ", " << rotation(1) << ", " << rotation(2) << ")";
    return out.str();
}