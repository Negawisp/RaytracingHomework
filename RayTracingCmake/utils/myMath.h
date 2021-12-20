#ifndef __MY_MATH_H__
#define __MY_MATH_H__

#include <cmath>
#include <limits>
#include <random>
#include "ray.h"
#include "vec.h"

const float epsilon = 1e-5f;
const float positiveInfinity = +std::numeric_limits<float>::infinity();
const float negativeInfinity = -std::numeric_limits<float>::infinity();
const float pi = 3.141593f;

inline float degreesToRadians(float degrees) {
    return degrees * pi / 180.0f;
}


inline float randomFloat(float min, float max) {
    static std::uniform_real_distribution<float> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float randomFloat() {
    return randomFloat(0.0f, 1.0f);
}

Vec3f randomInUnitBall();

Vec3f randomInUnitSphere();

Vec3f reflectVec(const Vec3f& v, const Vec3f n);

bool closeToZero(const Vec3f& v);

bool closeToZero(float a);

Vec3f multElementByElement(const Vec3f& v1, const Vec3f& v2);

float volume(const Vec3f& a, const Vec3f& b, const Vec3f& c);

#endif //__MY_MATH_H__