//
//  matrix.h
//  
//
//  Created by Akhil Batra on 3/9/14.
//
//

#ifndef ____matrix__
#define ____matrix__

#include "vec3.h"
#include <vector>
#include <string>
#include <sstream>

class Matrix {
private:
    // the actual table of values in this matrix
    vector<float> table;
    
    // height and width of the matrix
    int rows, cols;
    
    // helper to find determinant (does the recursion)
    float detHelper(Matrix m);
    
public:
    // default constructor
    Matrix();
    
    // constructor with given number of rows and columns
    Matrix(int r, int c);
    
    // converts the given vector into a 1 by 3 matrix
    Matrix(Vec3 v);
    
    // getter for the number of rows
    int getNumRows();
    
    // getter for the number of columns
    int getNumCols();
    
    // gets the value at the given x and y points
    float getValue(int r, int c);
    
    // sets the value at the given x and y to the given value
    void setValue(int r, int c, float value);
    
    // returns the inverse of this matrix
    Matrix inverse();
    
    // returns the transpose of this matrix
    Matrix transpose();
    
    // returns the determinant of this matrix
    float determinant();
    
    // checks to make sure if this matrix is a square matrix
    bool isSquare();
    
    // returns the resulting matrix of mulitplying the this matrix with the given one
    Matrix multiplyMatrix(Matrix m);
    
    // multiply every element in this array by the given float
    Matrix multiplyConst(float f);
    
    // returns the difference between this matrix and the given matrix
    Matrix subtract(Matrix m);
    
    // returns the sum of this matrix and the given matrix
    Matrix add(Matrix m);
    
    // converts this matrix into a string
    string stringify();
    
    // returns the Vec3 object that would technically be Vec4, but takes out the 4th element (it should be 1)
    Vec3 multiplyByVec4(Vec3 v, float w);
};

#endif /* defined(____matrix__) */