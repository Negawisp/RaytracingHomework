#ifndef __HIT_INFO_H__
#define __HIT_INFO_H__

#include "ray.h"

using std::shared_ptr;

class Hittable;
class Material;

struct HitInfo {
    Point3 point;
    Vec3f normal;
    float dist;
    bool frontFace;
    Color color;
    shared_ptr<Material> material;
    Hittable* body;

    inline void setFaceAndNormal(const Ray& ray, const Vec3f& frontNormal) {
        frontFace = ray.getDirection() * frontNormal < 0;
        normal = frontFace ? frontNormal : -frontNormal;
    }
};

#endif // !__HIT_INFO_H__
