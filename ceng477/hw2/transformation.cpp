//
// Created by deniz on 13.12.2020.
//

#include "transformation.h"

namespace fst {

    Transformation::Transformation(Translation t, math::Vector3f position) {
        pos = position;
        matrix = Matrix(transformator(t));
        calcPPrime();
        type = 't';
    }

    Transformation::Transformation(Scaling s, math::Vector3f position) {
        pos = position;
        matrix = transformator(s);
        calcPPrime();
        type = 's';
    }

    Transformation::Transformation(Rotation r, math::Vector3f position) {
        pos = position;
        matrix = transformator(r);
        calcPPrime();
        type = 'r';
    }

    fst::math::Vector4f Transformation::calculatePositionVector() {
        math::Vector4f p;
        p.x = pos.x;
        p.y = pos.y;
        p.z = pos.z;
        p.w = 1;
        return p;
    }

    void Transformation::calcPPrime() {
        p_prime = matrix.multiply(calculatePositionVector());
    }

    Matrix Transformation::transformator(fst::Translation x) {
        return x.translationMatrix;
    }

    Matrix Transformation::transformator(Scaling x) {
        double epsilon = 0.0000001;
        bool isAtOrigin = pos.x <= epsilon && pos.y <= epsilon && pos.z <= epsilon;
        if(isAtOrigin) {
            return x.resultMatrix();
        }
        else {
            fst::Translation minus_t = Translation(-1 * pos.x, -1 * pos.y, -1 * pos.z);
            fst::Translation t = Translation(pos.x, pos.y, pos.z);
            return (transformator(t).multiply(x.resultMatrix())).multiply(transformator(minus_t));
        }
    }

    Matrix Transformation::transformator(Rotation x) {
        return x.rotationMatrix;
    }






}

