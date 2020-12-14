//
// Created by deniz on 13.12.2020.
//

#include "translation.h"
#include "rotation.h"
#include "scaling.h"

#ifndef HW2_TRANSFORMATION_H
#define HW2_TRANSFORMATION_H

namespace fst {
    class Transformation {
    public:
        Matrix matrix;
        char type;
        fst::math::Vector3f pos;
        math::Vector4f p_prime;
        Transformation(Translation t, math::Vector3f position);
        Transformation(Scaling s, math::Vector3f position);
        Transformation(Rotation r, math::Vector3f position);
        math::Vector4f calculatePositionVector();
        void calcPPrime();
        Matrix transformator(Translation x);
        Matrix transformator(Scaling x);
        Matrix transformator(Rotation x);


    };
}



#endif //HW2_TRANSFORMATION_H
