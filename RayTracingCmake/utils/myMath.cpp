#ifndef __MY_MATH_CPP__
#define __MY_MATH_CPP__

#include "myMath.h"

/* Is as fast as an "honest" method */
Vec3f randomInUnitBall() {
    while (true)
    {
        Vec3f ret(randomFloat(-1.f, 1.f), randomFloat(-1.f, 1.f), randomFloat(-1.f, 1.f));
        if (ret.norm() < 1.f) {
            return ret;
        }
    }
}

Vec3f randomInUnitSphere() {
    return randomInUnitBall().normalize(1);
}

Vec3f reflectVec(const Vec3f& v, const Vec3f n) {
    return v - 2 * (v * n) * n;
}

bool closeToZero(const Vec3f& v) {
    float e = epsilon;
    return abs(v.x) < e && abs(v.y) < e && abs(v.z) < e;
}

bool closeToZero(float a) {
    return abs(a) < epsilon;
}

Vec3f multElementByElement(const Vec3f& v1, const Vec3f& v2) {
    return Vec3f(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

float volume(const Vec3f& a, const Vec3f& b, const Vec3f& c) {
    return a.x * (b.y * c.z - b.z * c.y) -
        a.y * (b.x * c.z - b.z * c.x) +
        a.z * (b.x * c.y - b.y * c.x);
}

#endif //__MY_MATH_CPP__