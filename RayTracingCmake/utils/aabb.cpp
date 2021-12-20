#ifndef __AABB_CPP__
#define __AABB_CPP__

#include "aabb.h"

AABB::AABB() {}
AABB::AABB(const Point3& min, const Point3& max) {
    _min = min; _max = max;
    if (closeToZero(_min.x - _max.x)) {
        _min.x -= epsilon;
        _max.x += epsilon;
    }
    if (closeToZero(_min.y - _max.y)) {
        _min.y -= epsilon;
        _max.y += epsilon;
    }
    if (closeToZero(_min.z - _max.z)) {
        _min.z -= epsilon;
        _max.z += epsilon;
    }
}

bool AABB::rayIntersects(const Ray& r, double tMin, double tMax) const {
    for (int i = 0; i < 3; i++) {
        auto t0 = fmin((_min[i] - r.getOrigin()[i]) / r.getDirection()[i],
                       (_max[i] - r.getOrigin()[i]) / r.getDirection()[i]);
        auto t1 = fmax((_min[i] - r.getOrigin()[i]) / r.getDirection()[i],
                       (_max[i] - r.getOrigin()[i]) / r.getDirection()[i]);
        tMin = fmax(t0, tMin);
        tMax = fmin(t1, tMax);
        if (tMax <= tMin)
            return false;
    }
    return true;
}


AABB bound(AABB& aabb1, AABB& aabb2) {
    Point3 min(
        fmin(aabb1.getMin().x, aabb2.getMin().x),
        fmin(aabb1.getMin().y, aabb2.getMin().y),
        fmin(aabb1.getMin().z, aabb2.getMin().z)
    );
    Point3 max(
        fmax(aabb1.getMax().x, aabb2.getMax().x),
        fmax(aabb1.getMax().y, aabb2.getMax().y),
        fmax(aabb1.getMax().z, aabb2.getMax().z)
    );

    return AABB(min, max);
}

#endif //__AABB_CPP__