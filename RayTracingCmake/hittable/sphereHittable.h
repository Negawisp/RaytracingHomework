#ifndef __SPHERE_MESH_H__
#define __SPHERE_MESH_H__

#include "hittable.h"
#include "../utils/vec.h"
#include "../utils/ray.h"

class SphereHittable : public Hittable {
public:
    SphereHittable(shared_ptr<Material> material, const Vec3f& center, const float& radius) :
        Hittable(material), _center(center), _radius(radius) {}

    bool rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) override;
    bool getBoundingBox(AABB& aabb) const;
    float distanse(const Point3& orig) const override;

private: 
    Vec3f _center;
    float _radius;
};


bool SphereHittable::rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) {
    Vec3f L = _center - ray.getOrigin();
    float tca = L * ray.getDirection();
    float d2 = L * L - tca * tca;
    if (d2 > _radius * _radius) {
        return false;
    }

    float thc = sqrtf(_radius * _radius - d2);
    float t = tca - thc;
    float t1 = tca + thc;
    if (t < 0) {
        t = t1;
    }
    if (t < 0) {
        return false;
    }
    if (tMin > t ||
        0 < tMax && tMax < t) {
        return false;
    }

    hitInfo.dist = t;
    hitInfo.material = _material;
    hitInfo.body = this;
    hitInfo.point = ray.at(hitInfo.dist);
    hitInfo.setFaceAndNormal(ray, (hitInfo.point - _center).normalize());
    return true;
}

float SphereHittable::distanse(const Point3& orig) const {
    Vec3f v = _center - orig;
    return v.norm() - _radius;
}

bool SphereHittable::getBoundingBox(AABB& aabb) const {
    Vec3f v(_radius, _radius, _radius);
    Point3 min = _center - v;
    Point3 max = _center + v;

    aabb = AABB(min, max);
    return true;
}

#endif //__SPHERE_MESH_H__