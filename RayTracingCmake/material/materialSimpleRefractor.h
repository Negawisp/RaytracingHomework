#ifndef __MATERIAL_DIELECTRIC_H__
#define __MATERIAL_DIELECTRIC_H__

#include "material.h"
#include "../utils/myMath.h"

class MaterialSimpleRefractor : public Material {
public:
    MaterialSimpleRefractor(float refractionIndex) : _refractionIndex(refractionIndex) {}

    bool refract(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& refracted) const override;
    bool refract2(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& refracted) const;

private:
    float _refractionIndex;
};

bool MaterialSimpleRefractor::refract(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& refracted) const {
    float refraction_ratio = hitInfo.frontFace ? (1.0f / _refractionIndex) : _refractionIndex;

    Vec3f v = incommingRay.getDirection();
    Vec3f n = hitInfo.normal;

    auto cos_theta = fmin((-v * n) / v.norm(), 1.0f);
    Vec3f r_out_perp = refraction_ratio * (v + cos_theta * n);
    Vec3f r_out_parallel = -sqrt(fabs(1.0 - r_out_perp * r_out_perp)) * n;

    refracted = Ray(hitInfo.point, r_out_perp + r_out_parallel);
    attenuation = Color(1.0f, 1.0f, 1.0f);
    return true;
}


#endif // !__MATERIAL_DIELECTRIC_H__
