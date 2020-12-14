#include "translation.h"

namespace fst
{
    Translation::Translation(float x, float y, float z)
    : x(x),
    y(y),
    z(z)
    {
        calculateTranslationMatrix();
    }

    void Translation::calculateTranslationMatrix() {
        translationMatrix.m[0][3] = x;
        translationMatrix.m[1][3] = y;
        translationMatrix.m[2][3] = z;

    }

    Matrix Translation::resultMatrix() {
        return this->translationMatrix;
    }


} // namespace fst
