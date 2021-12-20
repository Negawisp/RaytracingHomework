#ifndef __BOX_HITTABLE_H__
#define __BOX_HITTABLE_H__

#include "xyRectHittable.h"
#include "yzRectHittable.h"
#include "zxRectHittable.h"
#include "hittableList.h"

class BoxHittable : public Hittable {
public:
    BoxHittable(shared_ptr<Material> material, Point3& min, Point3& max);

    bool rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) override;
    bool getBoundingBox(AABB& aabb) const override;
    float distanse(const Point3& orig) const override;

protected:
    Point3 _min;
    Point3 _max;
    HittableList _sides;
};

BoxHittable::BoxHittable(shared_ptr<Material> material, Point3& min, Point3& max) {
    _material = material;
    _min = min;
    _max = max;

    _sides.add(make_shared<XYRectHittable>(material, _min.x, _min.y, _max.x, _max.y, _min.z));
    _sides.add(make_shared<XYRectHittable>(material, _min.x, _min.y, _max.x, _max.y, _max.z));

    _sides.add(make_shared<YZRectHittable>(material, _min.y, _min.z, _max.y, _max.z, _min.x));
    _sides.add(make_shared<YZRectHittable>(material, _min.y, _min.z, _max.y, _max.z, _max.x));

    _sides.add(make_shared<ZXRectHittable>(material, _min.z, _min.x, _max.z, _max.x, _min.y));
    _sides.add(make_shared<ZXRectHittable>(material, _min.z, _min.x, _max.z, _max.x, _max.y));
}

bool BoxHittable::rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) {
    bool ret = _sides.rayIntersects(ray, tMin, tMax, hitInfo);
    hitInfo.body = this;
    return ret;
}

bool BoxHittable::getBoundingBox(AABB& aabb) const {
    aabb._min = _min;
    aabb._max = _max;
    return true;
}

float BoxHittable::distanse(const Point3& orig) const {
    return _sides.distanse(orig);
}

#endif // __BOX_HITTABLE_H__
