#pragma once

namespace fst
{
    namespace math
    {
        struct Vector4f
        {
            float x, y, z, w;

            Vector4f() = default;
            explicit Vector4f(float a)
                : x(a)
                , y(a)
                , z(a)
                , w(a)
            {}
            Vector4f(float a, float b, float c, float d)
                : x(a)
                , y(b)
                , z(c)
                , w(d)
            {}
            float getElemWithIndex(int a) {
                switch (a) {
                    case 0:
                        return x;
                        break;

                    case 1:
                        return y;
                        break;

                    case 2:
                        return z;
                        break;

                    case 3:
                        return w;
                        break;
                    default:
                        return -1;
                        break;
                }
            }

            /*void setElemWithIndex(int ind, float val) {
                switch (ind) {
                    case 0:
                        x = val;
                        break;

                    case 1:
                        y = val;
                        break;

                    case 2:
                        z = val;
                        break;

                    case 3:
                        w = val;
                        break;
                    default:
                        break;
                }
            }*/

        };
    }
}