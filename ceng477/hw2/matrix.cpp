#include <stdexcept>
#include "matrix.h"

#include <iostream>
/*
 * m represents a 4x4 matrix stored as a 1D array in row-major-order.
 * For instance m[6] represents the element at the second row third column
 * of the matrix. 
 *
 * inv contains the inverse of this matrix stored in the same
 * format as the input.
 */
void Matrix::invert(double m[], double inv[])
{
    //
    // This code is taken from the GLU library 
    //    
    double det;

    inv[0] = m[5]  * m[10] * m[15] - 
        m[5]  * m[11] * m[14] - 
        m[9]  * m[6]  * m[15] + 
        m[9]  * m[7]  * m[14] +
        m[13] * m[6]  * m[11] - 
        m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
        m[4]  * m[11] * m[14] + 
        m[8]  * m[6]  * m[15] - 
        m[8]  * m[7]  * m[14] - 
        m[12] * m[6]  * m[11] + 
        m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
        m[4]  * m[11] * m[13] - 
        m[8]  * m[5] * m[15] + 
        m[8]  * m[7] * m[13] + 
        m[12] * m[5] * m[11] - 
        m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
        m[4]  * m[10] * m[13] +
        m[8]  * m[5] * m[14] - 
        m[8]  * m[6] * m[13] - 
        m[12] * m[5] * m[10] + 
        m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
        m[1]  * m[11] * m[14] + 
        m[9]  * m[2] * m[15] - 
        m[9]  * m[3] * m[14] - 
        m[13] * m[2] * m[11] + 
        m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
        m[0]  * m[11] * m[14] - 
        m[8]  * m[2] * m[15] + 
        m[8]  * m[3] * m[14] + 
        m[12] * m[2] * m[11] - 
        m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
        m[0]  * m[11] * m[13] + 
        m[8]  * m[1] * m[15] - 
        m[8]  * m[3] * m[13] - 
        m[12] * m[1] * m[11] + 
        m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
        m[0]  * m[10] * m[13] - 
        m[8]  * m[1] * m[14] + 
        m[8]  * m[2] * m[13] + 
        m[12] * m[1] * m[10] - 
        m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
        m[1]  * m[7] * m[14] - 
        m[5]  * m[2] * m[15] + 
        m[5]  * m[3] * m[14] + 
        m[13] * m[2] * m[7] - 
        m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
        m[0]  * m[7] * m[14] + 
        m[4]  * m[2] * m[15] - 
        m[4]  * m[3] * m[14] - 
        m[12] * m[2] * m[7] + 
        m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
        m[0]  * m[7] * m[13] - 
        m[4]  * m[1] * m[15] + 
        m[4]  * m[3] * m[13] + 
        m[12] * m[1] * m[7] - 
        m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
        m[0]  * m[6] * m[13] + 
        m[4]  * m[1] * m[14] - 
        m[4]  * m[2] * m[13] - 
        m[12] * m[1] * m[6] + 
        m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
        m[1] * m[7] * m[10] + 
        m[5] * m[2] * m[11] - 
        m[5] * m[3] * m[10] - 
        m[9] * m[2] * m[7] + 
        m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
        m[0] * m[7] * m[10] - 
        m[4] * m[2] * m[11] + 
        m[4] * m[3] * m[10] + 
        m[8] * m[2] * m[7] - 
        m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
        m[0] * m[7] * m[9] + 
        m[4] * m[1] * m[11] - 
        m[4] * m[3] * m[9] - 
        m[8] * m[1] * m[7] + 
        m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
        m[0] * m[6] * m[9] - 
        m[4] * m[1] * m[10] + 
        m[4] * m[2] * m[9] + 
        m[8] * m[1] * m[6] - 
        m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
    {
        throw std::runtime_error("Error: Determinant is zero.");
    }

    det = 1.0 / det;

    for (int i = 0; i < 16; ++i)
    {
        inv[i] *= det;
    }
}



Matrix Matrix::transpose() {
    Matrix result;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            result.m[i][j] = this->m[j][i];
        }
    }
    return result;
}

Matrix Matrix::multiply(Matrix b) {
    Matrix result;
    double num;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            num = 0;
            for(int k=0; k<4; k++) {
                num += this->m[i][k] * b.m[k][j];
            }
            result.m[i][j] = num;
        }
    }
    return result;
}

Matrix Matrix::add(Matrix b) {
    Matrix result;
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            result.m[i][j] += b.m[i][j];
        }
    }
    return result;
}



Matrix Matrix::inverse() {
    // todo
    return *this;
}


Matrix::Matrix() {
    // creates unit matrix
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            if (i != j) m[i][j] = 0;
            else m[i][j] = 1;
        }
    }
}

double Matrix::getIndex(int r, int c) {
    return this->m[r][c];
}

fst::math::Vector4f Matrix::multiply(fst::math::Vector4f v) {
    fst::math::Vector4f values;
    double total;

    for (int i = 0; i < 4; i++)
    {
        total = 0;
        for (int j = 0; j < 4; j++)
        {
            total += this->m[i][j] * v.getElemWithIndex(j);
        }
        switch (i) {
            case 0:
                values.x = total;
                break;
            case 1:
                values.y = total;
                break;
            case 2:
                values.z = total;
                break;
            case 3:
                values.w = total;
                break;
        }
        //values.setElemWithIndex(i, total);
    }
    return values;
}


Matrix::Matrix(Matrix const &x) {
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            this->m[i][j] = x.m[i][j];
        }
    }
}






