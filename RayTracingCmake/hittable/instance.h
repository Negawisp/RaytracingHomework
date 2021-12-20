#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include "hittable.h"
#include "../utils/matrix.h"

class Instance : public Hittable {
public:
    Instance(shared_ptr<Material> material, shared_ptr<Hittable> hittable, Vec3f& translation, float scale, Vec3f& rotationDir, float rotationAngle);

    bool rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo);
    bool getBoundingBox(AABB& aabb) const;
    float distanse(const Point3& orig) const;

private:
    shared_ptr<Hittable> _hittable;
    Mat4f _g2l;
    Mat4f _l2g;
    bool _hasBoundingBox;
    AABB _aabb;
};

Instance::Instance(shared_ptr<Material> material, shared_ptr<Hittable> hittable, Vec3f& translation, float scale, Vec3f& rotationDir, float rotationAngle) {
    _material = material;
    _hittable = hittable;
    _l2g = makeAffineMatrix(translation, scale, rotationDir, rotationAngle);
    _g2l = makeReverseAffineMatrix(translation, scale, rotationDir, rotationAngle);

    AABB aabbLocal;
    Point3 min(positiveInfinity, positiveInfinity, positiveInfinity);
    Point3 max(negativeInfinity, negativeInfinity, negativeInfinity);
    if (_hittable->getBoundingBox(aabbLocal)) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    float xLocal = i * aabbLocal.getMax().x + (1 - i) * aabbLocal.getMin().x;
                    float yLocal = j * aabbLocal.getMax().y + (1 - j) * aabbLocal.getMin().y;
                    float zLocal = k * aabbLocal.getMax().z + (1 - k) * aabbLocal.getMin().z;

                    Point3 pGlobal = transformPoint(_l2g, Point3(xLocal, yLocal, zLocal));

                    for (int c = 0; c < 3; c++) {
                        min[c] = fmin(min[c], pGlobal[c]);
                        max[c] = fmax(max[c], pGlobal[c]);
                    }
                }
            }
        }
    }
    _aabb = AABB(min, max);
}

bool Instance::rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) {
    Ray transformedRay = transformRay(_g2l, ray);
    if (!_hittable->rayIntersects(transformedRay, tMin, tMax, hitInfo)) {
        return false;
    }

    hitInfo.point = transformPoint(_l2g, hitInfo.point);
    hitInfo.setFaceAndNormal(ray, transformVec(_l2g, hitInfo.normal).normalize());
    hitInfo.material = _material;

    return true;
}

bool Instance::getBoundingBox(AABB& aabb) const {
    aabb = _aabb;
    return _hasBoundingBox;
}

float Instance::distanse(const Point3& orig) const {
    std::cerr << __FILE__ << __LINE__ << ": method not implemented." << std::endl;
    return positiveInfinity;
}

#endif //__INSTANCE_H__