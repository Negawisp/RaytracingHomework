#ifndef __XY_HITTABLE_H__
#define __XY_HITTABLE_H__

#include "hittable.h"

class XYRectHittable : public Hittable {
public:
    XYRectHittable(shared_ptr<Material> material, float x0, float y0, float x1, float y1, float z) :
        Hittable(material), _x0(x0), _y0(y0), _x1(x1), _y1(y1), _z(z) {}

    bool rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) override;
    bool getBoundingBox(AABB& aabb) const override;
    float distanse(const Point3& orig) const override;

protected:
    float _x0, _y0, _x1, _y1, _z;
};

bool XYRectHittable::rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) {
    float t = (_z - ray.getOrigin().z) / ray.getDirection().z;
    if (t > tMax || t < tMin) {
        return false;
    }
    float x = ray.getOrigin().x + ray.getDirection().x * t;
    float y = ray.getOrigin().y + ray.getDirection().y * t;

    if (x > _x1 || x < _x0  ||  y > _y1 || y < _y0) {
        return false;
    }

    hitInfo.body = this;
    hitInfo.material = _material;
    hitInfo.dist = t;
    hitInfo.setFaceAndNormal(ray, Vec3f(0.f, 0.f, 1.f));
    hitInfo.point = Vec3f(x, y, _z);
    return true;
}

bool XYRectHittable::getBoundingBox(AABB& aabb) const {
    aabb._min = Vec3f(_x0, _y0, _z - 0.0001f);
    aabb._max = Vec3f(_x1, _y1, _z + 0.0001f);
    return true;
}

float XYRectHittable::distanse(const Point3& orig) const {
    return fabs(orig.z - _z);
}

#endif //__XY_HITTABLE_H__