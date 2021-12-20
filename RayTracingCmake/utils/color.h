#ifndef __COLOR_H__
#define __COLOR_H__

#include "vec.h"

typedef vec<3, float> Color;

void write_color(Vec3c& buffer, const Vec3f& color) {
    float scale = 1.0f;
    float r = sqrt(scale * color.x);
    float g = sqrt(scale * color.y);
    float b = sqrt(scale * color.z);

    buffer = Vec3c(r * 255, g * 255, b * 255);
}

#endif //__COLOR_H__