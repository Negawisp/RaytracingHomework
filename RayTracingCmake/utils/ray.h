#ifndef RAY_H
#define RAY_H

#include "vec.h"

class Ray {
public:
    Ray() {}
    Ray(const Point3& origin, const Vec3f& direction)
        : _orig(origin), _dir(direction)
    {}

    Point3 getOrigin() const { return _orig; }
    Vec3f getDirection() const { return _dir; }

    Point3 at(float t) const {
        return _orig + _dir * t;
    }

private:
    Point3 _orig;
    Vec3f _dir;

};

#endif