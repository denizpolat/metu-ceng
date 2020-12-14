#ifndef __matrix_h__
#define __matrix_h__

#include "vector4f.h"

    class Matrix {
    private:
    public:
        double m[4][4];
        Matrix();
        Matrix(Matrix const &x);
        Matrix add(Matrix b);
        Matrix multiply(Matrix b);
        fst::math::Vector4f multiply(fst::math::Vector4f v);
        Matrix transpose();
        Matrix inverse();
        void invert(double m[], double inv[]);
        double getIndex(int r, int c);
        //~Matrix();


    };


#endif //__matrix_h__
