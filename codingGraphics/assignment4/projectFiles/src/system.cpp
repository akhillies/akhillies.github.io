//
//  system.cpp
//  
//
//  Created by Akhil Batra on 5/7/14.
//
//

#include "system.h"

System::System():System(0) {}

System::System(float err)
{
    error = err;
    endEffector = Vector3f(0, 0, 0);
    length = 0;
}

Matrix3Xf System::createJacobian()
{
    MatrixXf jacobian(3, 3 * joints.size());
    Matrix3f rot = identityMatrix(3);
//    Vector3f end = endEffector;
    int column = 0;
    for(int j = 0; j < joints.size(); j++)
    {
//        Matrix3f partJacob = rot * joints[j].getJacobian(end);
        Matrix3f partJacob = rot * joints[j].getJacobian();
        
        jacobian(0, column) = partJacob(0, column % 3);
        jacobian(1, column) = partJacob(1, column % 3);
        jacobian(2, column) = partJacob(2, column % 3);
        column++;
        jacobian(0, column) = partJacob(0, column % 3);
        jacobian(1, column) = partJacob(1, column % 3);
        jacobian(2, column) = partJacob(2, column % 3);
        column++;
        jacobian(0, column) = partJacob(0, column % 3);
        jacobian(1, column) = partJacob(1, column % 3);
        jacobian(2, column) = partJacob(2, column % 3);
        column++;
        
//        end -= joints[j].getEndPoint();
        rot *= joints[j].getRotationMatrix();
//        end -= rot * joints[j].getEndPoint();
    }
    return jacobian;
}

void System::update(VectorXf dr)
{
    int row = 0;
    
    endEffector = Vector3f(0, 0, 0);
    Matrix3f rotate = identityMatrix(3);
    
    for(int j = 0; j < joints.size(); j++)
    {
        Vector3f r;
        r(0) = dr(row);
        r(1) = dr(row + 1);
        r(2) = dr(row + 2);
//        cout << r << endl << endl;
//        j.update(p, r);
        
//        joints[j].update(rotate * r);
        joints[j].update(r);
        
        
        endEffector += rotate * joints[j].getEndPoint();
        rotate *= joints[j].getRotationMatrix();
        
        row += 3;
    }
}

int System::numJoints()
{
    return joints.size();
}

float System::getLength()
{
    return length;
}

float System::getError()
{
    return error;
}

Vector3f System::getEndEffector()
{
    return endEffector;
}

void System::setError(float err)
{
    error = err;
}

void System::addJoint(Joint j)
{
//    Matrix3f rot;
//    rot <<  1, 0, 0,
//            0, 1, 0,
//            0, 0, 1;
//
//    for(int j = 0; j < joints.size(); j++)
//    {
//        rot *= rot * joints[j].getRotationMatrix();
//        cout << "being added:" << endl << joints[j].getRotationMatrix() << endl << endl;
//        cout << rot << endl << endl;
//        cout << rot * rot.transpose() << endl << endl;
//    }
//    endEffector += rot * joints.back().getEndPoint();
//    cout << "add " << j.stringify() << endl << endl;
//    cout << rot << endl << endl;
//    cout << rot * rot.transpose() << endl << endl;
//    
//    cout << "gjkg" << endl << rot * joints.back().getEndPoint() << endl << endl << endl;
    
    Matrix3f rot = identityMatrix(3);
    for(int j = 0; j < joints.size(); j++)
    {
        rot *= joints[j].getRotationMatrix();
    }
    joints.push_back(Joint(j));
    endEffector += rot * joints.back().getEndPoint();
    length += joints.back().getLength();
}

void System::drawJoints()
{
    if(joints.size() > 0)
    {
//        joints[0].draw(Vector3f(0, 0, 0));
        joints[0].draw();
        for(int i = 1; i < joints.size(); i++)
        {
//            joints[i].draw(joints[i - 1].getEndEffector());
            joints[i].draw();
        }
    }
    
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GL_SMOOTH);
    gluQuadricDrawStyle(quad, GLU_FILL);
    gluSphere(quad, .025, 32, 32);
    for(int i = 0; i < joints.size(); i++)
    {
        glPopMatrix();
    }
}

Joint& System::operator[](int i)
{
    return joints[i];
}

MatrixXf System::identityMatrix(int n)
{
    MatrixXf i(n, n);
    for(int j = 0; j < n; j++)
    {
        for(int k = 0; k < n; k++)
        {
            i(j, k) = (j == k);
        }
    }
    return i;
}

bool System::applyIK(Vector3f goal)
{
    Vector3f newG = goal;
    
    if(length < goal.norm())
    {
        newG = goal.normalized() * length;
    }
    
//    JointRotations jntRots;
//    Vector3f de = goal - joints.back().getEndEffector();
    Vector3f de = newG - endEffector;
//    int i = 0;
//    float dist = de.norm();
    float dist = de.norm();//fmin(de.norm(), 1);
    if(dist > error)
//    while(dist > error)
    {
//        de = de.normalized() * dist;
//        cout << de << endl << endl;
        
        MatrixXf J = createJacobian();
//        MatrixXf Jplus = J.transpose() * (J * J.transpose()).inverse();
        MatrixXf Jplus = J.transpose() * (J * J.transpose() + 1 * identityMatrix(3)).inverse();
//        MatrixXf Jplus = J.transpose();
//        MatrixXf Jplus = J * J.transpose();
//        if(Jplus.determinant() == 0)
//        {
//            Jplus = J.transpose();
//        }
//        else
//        {
//            Jplus = J.transpose() * (Jplus + 20 * identityMatrix(3)).inverse();
//        }
//        cout << "ORIGINAL:\n" << (J) << endl << endl;
//        cout << "TRANSPOSE:\n" <<  (J.transpose()) << endl << endl;
//        cout << "TOGETHER:\n" <<  (J * J.transpose()) << endl << endl;
//        cout << "DETERMINANT:   " <<  (J * J.transpose()).determinant() << endl << endl << endl;
        
        VectorXf dr = Jplus * (0.25 * de);
        update(dr);
        
//        de = goal - joints.back().getEndEffector();
//        de = goal - endEffector;
//        dist = de.norm();//fmin(de.norm(), 1);
//        de = de.normalized() * dist;
//        drawJoints();
//        i++;
        return false;
    }
//    cout << i << endl;
    cout << "At goal: " << endl;
    return true;
//    return jntRots;
}