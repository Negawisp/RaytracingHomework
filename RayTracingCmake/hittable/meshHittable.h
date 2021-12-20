#ifndef __MESH_HITTABLE_H__
#define __MESH_HITTABLE_H__

#include "hittable.h"
#include "cmesh.h"
#include "utils/linal.h"
#include "../material/material.h"

using std::shared_ptr;

class MeshHittable : public Hittable {
public:
    MeshHittable(shared_ptr<cmesh::SimpleMesh> mesh, shared_ptr<Material> material);

    bool rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) override;
    bool getBoundingBox(AABB& aabb) const override;
    float distanse(const Vec3f& orig) const override;

private:
    shared_ptr<cmesh::SimpleMesh> _mesh;
};

MeshHittable::MeshHittable(shared_ptr<cmesh::SimpleMesh> mesh, shared_ptr<Material> material) {
    _mesh = mesh;
    _material = material;
}

bool MeshHittable::rayIntersects(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) {
    HitInfo temp;
    bool collisionDetected = false;
    hitInfo.dist = tMax;

    unsigned int _trianglesN = _mesh->TrianglesNum();
    std::vector<float>& pos  = _mesh->vPos4f;
    std::vector<float>& norm = _mesh->vNorm4f;

    // For each triangle
    for (int i = 0; i < _trianglesN; i++) {

        unsigned int index0 = _mesh->indices[i * 3 + 0] * 4;
        unsigned int index1 = _mesh->indices[i * 3 + 1] * 4;
        unsigned int index2 = _mesh->indices[i * 3 + 2] * 4;
        
        Point3 p1(pos[index0], pos[index0 + 1], pos[index0 + 2]);
        Point3 p2(pos[index1], pos[index1 + 1], pos[index1 + 2]);
        Point3 p3(pos[index2], pos[index2 + 1], pos[index2 + 2]);

        Vec3f n1(norm[index0], norm[index0 + 1], norm[index0 + 2]);
        Vec3f n2(norm[index1], norm[index1 + 1], norm[index1 + 2]);
        Vec3f n3(norm[index2], norm[index2 + 1], norm[index2 + 2]);

        Vec3f d2 = p2 - p1;
        Vec3f d3 = p3 - p1;

        if (closeToZero(volume(d2, d3, ray.getDirection()))) {
            continue;
        }

        Vec3f answer;
        solve(makeMatrixFromColumns(ray.getDirection(), p1 - p2, p1 - p3), p1 - ray.getOrigin(), answer);
        
        float t = answer.x;
        float u = answer.y;
        float v = answer.z;

        if (u < 0 || v < 0 || u + v > 1) {
            continue;
        }
        if (t > hitInfo.dist || t < tMin) {
            continue;
        }

        collisionDetected = true;
        Vec3f normal = (1.f - u - v) * n1 + u * n2 + v * n3;
//        Vec3f normal  = (n1 + n2 + n3) * (1.f / 3.f);
        hitInfo.dist = t;
        hitInfo.body = this;
        hitInfo.material = _material;
        hitInfo.point = ray.at(t);
        hitInfo.setFaceAndNormal(ray, normal);
    }

    return collisionDetected;
}

bool MeshHittable::getBoundingBox(AABB& aabb) const {
    aabb = _mesh->getBoundingBox();
    return true;
}

float MeshHittable::distanse(const Vec3f& orig) const {
    return positiveInfinity;
}

#endif //__MESH_HITTABLE_H__