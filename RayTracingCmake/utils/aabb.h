#ifndef __AABB_H__
#define __AABB_H__

#include "myMath.h"
#include "vec.h"
#include "ray.h"

class AABB {
public:
    AABB();
    AABB(const Point3& min, const Point3& max);

    Point3 getMin() const { return _min; }
    Point3 getMax() const { return _max; }

    bool rayIntersects(const Ray& r, double tMin, double tMax) const;

    Point3 _min;
    Point3 _max;
};

AABB bound(AABB& aabb1, AABB& aabb2);

#endif //__AABB_H__