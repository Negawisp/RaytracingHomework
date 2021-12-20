#ifndef __ZX_HITTABLE_H__
#define __ZX_HITTABLE_H__

#include "hittable.h"

class ZXRectHittable : public Hittable {
public:
    ZXRectHittable(shared_ptr<Material> material, float z0, float x0, float z1, float x1, float y) :
        Hittable(material), _z0(z0), _x0(x0), _z1(z1), _x1(x1), _y(y) {}

    bool rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) override;
    bool getBoundingBox(AABB& aabb) const override;
    float distanse(const Point3& orig) const override;

protected:
    float _z0, _x0, _z1, _x1, _y;
};

bool ZXRectHittable::rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) {
    float t = (_y - ray.getOrigin().y) / ray.getDirection().y;
    if (t > tMax || t < tMin) {
        return false;
    }
    float z = ray.getOrigin().z + ray.getDirection().z * t;
    float x = ray.getOrigin().x + ray.getDirection().x * t;

    if (z > _z1 || z < _z0 || x > _x1 || x < _x0) {
        return false;
    }

    hitInfo.body = this;
    hitInfo.material = _material;
    hitInfo.dist = t;
    hitInfo.setFaceAndNormal(ray, Vec3f(0.f, 1.f, 0.f));
    hitInfo.point = Vec3f(x, _y, z);
    return true;
}

bool ZXRectHittable::getBoundingBox(AABB& aabb) const {
    aabb._min = Vec3f(_x0, _y - 0.0001f, _z0);
    aabb._max = Vec3f(_x1, _y + 0.0001f, _z1);
    return true;
}

float ZXRectHittable::distanse(const Point3& orig) const {
    return fabs(orig.y - _y);
}

#endif //__ZX_HITTABLE_H__