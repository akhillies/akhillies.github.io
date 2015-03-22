//
//  matrix.cpp
//  
//
//  Created by Akhil Batra on 3/9/14.
//
//

#include "matrix.h"

Matrix::Matrix() {
    rows = 0;
    cols = 0;
    table.resize(getNumRows() * getNumCols());
}

Matrix::Matrix(int r, int c) {
    rows = r;
    cols = c;
    table.resize(getNumRows() * getNumCols());
}

Matrix::Matrix(Vec3 v) {
    rows = 1;
    cols = 3;
    table.resize(getNumRows() * getNumCols());
    setValue(0, 0, v.getX());
    setValue(0, 1, v.getY());
    setValue(0, 2, v.getZ());
}

int Matrix::getNumRows() {
    return rows;
}

int Matrix::getNumCols() {
    return cols;
}

float Matrix::getValue(int r, int c) {
    return table[c + r * getNumCols()];
}

void Matrix::setValue(int r, int c, float value) {
    table[c + r * getNumCols()] = value;
}

bool Matrix::isSquare() {
    return getNumCols() == getNumRows();
}

Vec3 Matrix::multiplyByVec4(Vec3 v, float w) {
    Matrix v4(4, 1);
    v4.setValue(0, 0, v.getX());
    v4.setValue(1, 0, v.getY());
    v4.setValue(2, 0, v.getZ());
    v4.setValue(3, 0, w);
    
    Matrix transV = this->multiplyMatrix(v4);
    return Vec3(transV.getValue(0, 0), transV.getValue(1, 0), transV.getValue(2, 0));
}

Matrix Matrix::transpose() {
    Matrix m(getNumRows(), getNumCols());
    for(int i = 0; i < getNumRows(); i++) {
        for(int j = 0; j < getNumCols(); j++) {
            m.setValue(i, j, getValue(j, i));
        }
    }
    return m;
}


float Matrix::detHelper(Matrix m) {
    double det = 0;
    if(m.getNumRows() == 1) {
        return m.getValue(0, 0);
    }
    else if(m.getNumRows() == 2) {
        return m.getValue(0, 0) * m.getValue(1, 1) - m.getValue(1, 0) * m.getValue(0, 1);
    }
    else {
        for (int c = 0; c < m.getNumCols(); c++) {
            Matrix newM(m.getNumRows() - 1, m.getNumCols() - 1);
            for(int ro = 1; ro < m.getNumRows(); ro++) {
                int passCol = 0;
                for(int co = 0; co < m.getNumCols(); co++) {
                    if(co == c) {
                        passCol = 1;
                        continue;
                    }
                    newM.setValue(ro - 1, co - passCol, m.getValue(ro, co));
                }
            }
            float f = (-2 * (c % 2) + 1) * m.getValue(0, c) * detHelper(newM);
            det += f;
        }
    }
    return det;
}

float Matrix::determinant() {
    if(isSquare()) {
        return detHelper(*this);
    }
    cout << "FAILURE, YOU TRIED TO TAKE DETERMINANT OF NON-SQUARE MATRIX";
    return 0;
}

Matrix Matrix::inverse() {
    Matrix inv(getNumCols(), getNumRows());
    double determ = determinant();
    if (determ != 0) {
        if (getNumRows() == 1) {
            inv.setValue(0, 0, 1 / getValue(0, 0));
        }
        else if (getNumRows() == 2) {
            inv.setValue(0, 0, getValue(1, 1));
            inv.setValue(0, 1, -getValue(0, 1));
            inv.setValue(1, 0, -getValue(1, 0));
            inv.setValue(1, 1, getValue(1, 1));
            inv.multiplyConst(1 / determ);
        }
        else {
            for(int i = 0; i < getNumRows(); i++) {
                inv.setValue(i, i, 1);
            }
            
            Matrix copy(*this);
            for (int c = 0; c < getNumCols(); c++) {
                int r = c;
                while(r < getNumRows() && copy.getValue(r, c) == 0) {
                    r++;
                }
                if (r != c) {
                    for (int s = 0; s < getNumCols(); s++) {
                        float temp = copy.getValue(c, s);
                        copy.setValue(c, s, copy.getValue(r, s));
                        copy.setValue(r, s, temp);
                        temp = inv.getValue(c, s);
                        inv.setValue(c, s, inv.getValue(r, s));
                        inv.setValue(r, s, temp);
                    }
                }
                for (int r = 0; r < getNumRows(); r++) {
                    if(r != c) {
                        if (copy.getValue(r, c) != 0) {
                            float f = -copy.getValue(r, c) / copy.getValue(c, c);
                            for (int s = 0; s < getNumCols(); s++) {
                                copy.setValue(r, s, copy.getValue(r, s) + f * copy.getValue(c, s));
                                inv.setValue(r, s, inv.getValue(r, s) + f * inv.getValue(c, s));
                            }
                        }
                    }
                    else {
                        float f = copy.getValue(c, c);
                        for (int s = 0; s < getNumCols(); s++) {
                            copy.setValue(r, s, copy.getValue(r, s) / f);
                            inv.setValue(r, s, inv.getValue(r, s) / f);
                        }
                    }
                }
            }
        }
    }
    else {
        cout << "IDIOT, TAKING AN INVERSE OF MATRIX WITH DETERMINANT OF 0";
    }
    return inv;
}


Matrix Matrix::multiplyMatrix(Matrix m) {
    Matrix prod(getNumRows(), m.getNumCols());
    for(int i = 0; i < getNumRows(); i++) {
        for(int j = 0; j < m.getNumCols(); j++) {
            float total = 0;
            for(int k = 0; k < getNumCols() && k < m.getNumRows(); k++) {
                total += getValue(i, k) * m.getValue(k, j);
            }
            prod.setValue(i, j, total);
        }
    }
    return prod;
}

Matrix Matrix::multiplyConst(float f) {
    Matrix m(getNumRows(), getNumCols());
    for(int i = 0; i < getNumRows(); i++) {
        for(int j = 0; j < getNumCols(); j++) {
            m.setValue(i, j, getValue(i, j) * f);
        }
    }
    return m;
}

Matrix Matrix::subtract(Matrix m) {
    Matrix diff(getNumRows(), getNumCols());
    for(int i = 0; i < getNumRows(); i++) {
        for(int j = 0; j < getNumCols(); j++) {
            diff.setValue(i, j, getValue(i, j) - m.getValue(i, j));
        }
    }
    return diff;
}

Matrix Matrix::add(Matrix m) {
    Matrix sum(getNumRows(), getNumCols());
    for(int i = 0; i < getNumRows(); i++) {
        for(int j = 0; j < getNumCols(); j++) {
            sum.setValue(i, j, getValue(i, j) + m.getValue(i, j));
        }
    }
    return sum;
}

string Matrix::stringify() {
    ostringstream s;
    for(int i = 0; i < getNumRows(); i++) {
        s << "| ";
        for(int j = 0; j < getNumCols(); j++) {
            s << getValue(i, j) << " ";
        }
        s << "|\n";
    }
    return s.str();
}
