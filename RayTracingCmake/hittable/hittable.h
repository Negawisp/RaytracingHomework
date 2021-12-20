#ifndef __MESH_H__
#define __MESH_H__

#include "../utils/ray.h"
#include "../utils/color.h"
#include "../material/material.h"
#include "../utils/aabb.h"

using std::shared_ptr;

struct HitInfo;

class Hittable {
public:
    Hittable() {};
    Hittable(shared_ptr<Material> material) : _material(material) {}
    ~Hittable() {}

    shared_ptr<Material> getMaterial() { return _material; }

    virtual bool rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) = 0;
    virtual bool getBoundingBox(AABB& aabb) const = 0;
    virtual float distanse(const Point3& orig) const = 0;

protected:
    shared_ptr<Material> _material;
};

#endif //__MESH_H__