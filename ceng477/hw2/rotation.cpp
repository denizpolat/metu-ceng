#include "rotation.h"

#include <cmath>

namespace fst
{



    Matrix Rotation::calculateRotationMatrix(char rotationAxe, double sin_theta, double cos_theta, int isCCW) {
        Matrix matrix;
        switch(rotationAxe) {
            case ('x'):
                matrix.m[1][1] = cos_theta;
                matrix.m[1][2] = -1 * sin_theta * isCCW;
                matrix.m[2][1] = sin_theta * isCCW;
                matrix.m[2][2] = cos_theta;
                break;
            case ('y'):
                matrix.m[0][0] = cos_theta;
                matrix.m[0][2] = sin_theta * isCCW;
                matrix.m[2][0] = -1 * sin_theta * isCCW;
                matrix.m[2][2] = cos_theta;
                break;
            case ('z'):
                matrix.m[0][0] = cos_theta;
                matrix.m[0][1] = -1 * sin_theta * isCCW;
                matrix.m[1][0] = sin_theta * isCCW;
                matrix.m[1][1] = cos_theta;
            default:
                break;

        }
        return matrix;
    }

    Rotation::Rotation(float a, float x, float y, float z) :
    angle(a) ,
    x(x) ,
    y(y) ,
    z(z)
    {
        setFinalRotationMatrix();
    }

    void Rotation::setFinalRotationMatrix() {
        math::Vector3f rotationAxis = math::Vector3f(this->x, this->y, this->z);
        math::Vector3f normalizedU = math::normalize(rotationAxis);

        double eps = 0.000001;

        double sin_theta = sin(angle * M_PI / 180.);
        double cos_theta = cos(angle * M_PI / 180.);


        // rotation axis is already one of x,y,z axes
        if(rotationAxis.x <= eps && rotationAxis.y <= eps)  {
            rotationMatrix = calculateRotationMatrix('z', sin_theta, cos_theta, 1);
            return;
        }

        else if(rotationAxis.z <= eps && rotationAxis.y <= eps)  {
            rotationMatrix = calculateRotationMatrix('x', sin_theta, cos_theta, 1);
            return;
        }

        else if(rotationAxis.x <= eps && rotationAxis.z <= eps)  {
            rotationMatrix = calculateRotationMatrix('y', sin_theta, cos_theta, 1);
            return;
        }
        // slayt34 - 288

        // rotate around x to bring u onto xz plane (ccw)

        math::Vector3f u_prime = math::Vector3f(0, normalizedU.y, normalizedU.z);
        double u_y = normalizedU.y;
        double u_z = normalizedU.z;
        double u_prime_length = sqrt(pow(u_y, 2) + pow(u_z, 2));
        double cos_alpha = u_z / u_prime_length;
        double sin_alpha = u_y / u_prime_length;

        // rotate around y to align the result with z axis (cw)

        double cos_beta = sqrt(pow(u_y, 2) + pow(u_z, 2));
        double sin_beta = normalizedU.x;




        // todo slaytlarda Ry beta ların yönünde sıkıntı var gibi, check


        Matrix rx_alpha = calculateRotationMatrix('x', sin_alpha, cos_alpha, 1);

        Matrix ry_beta = calculateRotationMatrix('y', sin_beta, cos_beta, 1);

        Matrix rx_minus_alpha = calculateRotationMatrix('x', sin_alpha, cos_alpha, -1);

        Matrix ry_minus_beta = calculateRotationMatrix('y', sin_beta, cos_beta, -1);

        Matrix rz_theta = calculateRotationMatrix('z', sin_theta, cos_theta, 1);

        rotationMatrix = (((rx_minus_alpha.multiply(ry_beta)).multiply(rz_theta)).multiply(ry_minus_beta)).multiply(rx_alpha);

    }

    Matrix Rotation::resultMatrix() {
        return rotationMatrix;
    }


}

