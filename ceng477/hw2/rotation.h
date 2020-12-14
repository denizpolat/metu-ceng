#pragma once

#include "matrix.h"
#include "vector3f.h"

namespace fst
{
    class Rotation
    {

    public:
        float angle, x, y, z;
        Matrix rotationMatrix;
        Matrix calculateRotationMatrix(char axis, double sina, double cosa, int isCCW);
        Rotation(float angle, float x, float y, float z);
        void setFinalRotationMatrix();
        Matrix resultMatrix();
    };
} // namespace fst
