#include "scaling.h"

namespace fst
{
    Scaling::Scaling(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {
        calculateScalingMatrix();
    }

    void Scaling::calculateScalingMatrix() {
        scalingMatrix.m[0][0] = x;
        scalingMatrix.m[1][1] = y;
        scalingMatrix.m[2][2] = z;

    }

    Matrix Scaling::resultMatrix() {
        // todo transform if needed, then scale (YOU NEED POSITION)
        return scalingMatrix;
    }

    double Scaling::sphereScaling(double radius) {
        double epsilon = 0.000001;
        if(abs(this->x - this->y) < epsilon && abs(this->y - this->z) < epsilon) {
            return radius * this->x;
        }
        else return -1;
    }

}
