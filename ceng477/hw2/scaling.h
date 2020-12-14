#pragma once

#include "matrix.h"
#include "vector3f.h"


namespace fst
{
    class Scaling
    {
    private:
        Matrix scalingMatrix;

    public:
        float x, y, z;
        void calculateScalingMatrix();
        Scaling(float x, float y, float z);
        Matrix resultMatrix();
        double sphereScaling(double radius);
    };
}
