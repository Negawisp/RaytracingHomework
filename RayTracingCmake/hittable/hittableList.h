#ifndef __HITTABLE_LIST_H__
#define __HITTABLE_LIST_H__

#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
public:
    HittableList() : Hittable() {};
    HittableList(shared_ptr<Hittable> hittable) { add(hittable); }

    void add(shared_ptr<Hittable> hittable) { _hittables.push_back(hittable); }
    void clear() { _hittables.clear(); }

    bool rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) override;
    bool getBoundingBox(AABB& aabb) const;
    float distanse(const Vec3f& orig) const;

private:
    std::vector<shared_ptr<Hittable>> _hittables;
};


bool HittableList::rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) {
    HitInfo hitTemp = {};
    bool didHit = false;
    float closestDist = tMax;

    for each (shared_ptr<Hittable> hittable in _hittables)
    {
        AABB aabb;
        if (hittable->getBoundingBox(aabb) &&
            aabb.rayIntersects(ray, tMin, closestDist) &&
            hittable->rayIntersects(ray, tMin, closestDist, hitTemp)) {
            didHit = true;
            closestDist = hitTemp.dist;
            hitInfo = hitTemp;
        }
    }

    return didHit;
}

float HittableList::distanse(const Vec3f& orig) const {
    float dist = -1.f;
    float tempDist = -1.f;

    for each (shared_ptr<Hittable> hittable in _hittables) {
        tempDist = hittable->distanse(orig);
        if (tempDist < dist || dist < -1) {
            dist = tempDist;
        }
    }

    return dist;
}

bool HittableList::getBoundingBox(AABB& output) const {
    if (_hittables.empty()) {
        return false;
    }

    AABB tempBox;
    bool firstBox = true;

    for (const auto& hittable : _hittables) {
        if (!hittable->getBoundingBox(tempBox)) {
            continue;
        }
        output = firstBox ? tempBox : bound(output, tempBox);
        firstBox = false;
    }
}

#endif // !__HITTABLE_LIST_H__
