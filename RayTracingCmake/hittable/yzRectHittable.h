#ifndef __YZ_HITTABLE_H__
#define __YZ_HITTABLE_H__

#include "hittable.h"

class YZRectHittable : public Hittable {
public:
    YZRectHittable(shared_ptr<Material> material, float y0, float z0, float y1, float z1, float x) :
        Hittable(material), _y0(y0), _z0(z0), _y1(y1), _z1(z1), _x(x) {}

    bool rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) override;
    bool getBoundingBox(AABB& aabb) const override;
    float distanse(const Point3& orig) const override;

protected:
    float _y0, _z0, _y1, _z1, _x;
};

bool YZRectHittable::rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) {
    float t = (_x - ray.getOrigin().x) / ray.getDirection().x;
    if (t > tMax || t < tMin) {
        return false;
    }
    float y = ray.getOrigin().y + ray.getDirection().y * t;
    float z = ray.getOrigin().z + ray.getDirection().z * t;

    if (y > _y1 || y < _y0 || z > _z1 || z < _z0) {
        return false;
    }

    hitInfo.body = this;
    hitInfo.material = _material;
    hitInfo.dist = t;
    hitInfo.setFaceAndNormal(ray, Vec3f(1.f, 0.f, 0.f));
    hitInfo.point = Vec3f(_x, y, z);
    return true;
}

bool YZRectHittable::getBoundingBox(AABB& aabb) const {
    aabb._min = Vec3f(_x - 0.0001f, _y0, _z0);
    aabb._max = Vec3f(_x + 0.0001f, _y1, _z1);
    return true;
}

float YZRectHittable::distanse(const Point3& orig) const {
    return fabs(orig.x - _x);
}

#endif //__YZ_HITTABLE_H__