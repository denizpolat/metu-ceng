#pragma once

#include "matrix.h"
#include "vector3f.h"

namespace fst
{
    class Translation
    {
    public:
        float x, y, z;
        Matrix translationMatrix;
        void calculateTranslationMatrix();
        Translation(float x, float y, float z);
        Matrix resultMatrix();
    };
} // namespace fst
