#ifndef __MATERIAL_FRENEL_H__
#define __MATERIAL_FRENEL_H__

#include "material.h"

class MaterialPureFrenel : public Material {
public:
    MaterialPureFrenel(float refractionIndex, Color albedo) : _refractionIndex(refractionIndex), _albedo(albedo) {}
    
    bool reflect(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& reflected) const override;
    bool refract(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& refracted) const override;

private:
    float _refractionIndex;
    Color _albedo;
};

bool MaterialPureFrenel::reflect(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& reflected) const {
    Vec3f v_i = incommingRay.getDirection();
    Vec3f n = hitInfo.normal;
    float reflectionRatio;


    float cos_i = fmin((-v_i * n) / v_i.norm(), 1.0f);
    float sin_i = sqrt(1 - cos_i * cos_i);

    if (!hitInfo.frontFace) {
        float crit_sin = 1.f / _refractionIndex;
        attenuation = Color(1.f, 1.f, 1.f);
        if (sin_i >= crit_sin) {
            reflected = Ray(hitInfo.point, reflectVec(v_i, n));
            return true;
        }
        reflectionRatio = crit_sin;
    }
    else {
        attenuation = _albedo;
        reflectionRatio = _refractionIndex;
    }

    float sin_t = sin_i / reflectionRatio;
    float cos_t = sqrt(1 - sin_t * sin_t);

    float r_s = (cos_i - reflectionRatio * cos_t) /
                (cos_i + reflectionRatio * cos_t);
    float r_p = (reflectionRatio * cos_i - cos_t) /
                (reflectionRatio * cos_i + cos_t);
    float R = (r_s * r_s + r_p * r_p) / 2;

    attenuation = multElementByElement(Color(R, R, R), attenuation);
    reflected = Ray(hitInfo.point, reflectVec(v_i, n));
    return true;
}

bool MaterialPureFrenel::refract(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& refracted) const {

    Vec3f v_i = incommingRay.getDirection();
    Vec3f n = hitInfo.normal;
    float refractionRatio;

    float cos_i = fmin((-v_i * n) / v_i.norm(), 1.0f);
    float sin_i = sqrt(1 - cos_i * cos_i);
    
    if (!hitInfo.frontFace) {
        if (sin_i >= 1.f / _refractionIndex) {
            return false;
        }
        attenuation = Color(1., 1., 1.);
        refractionRatio = _refractionIndex;
    } else {
        attenuation = _albedo;
        refractionRatio = 1.f / _refractionIndex;
    }

    float sin_t = refractionRatio * sin_i;
    float cos_t = sqrt(1 - sin_t * sin_t);


    Vec3f r_out_perp = refractionRatio * (v_i + cos_i * n);
    Vec3f r_out_parallel = -sqrt(fabs(1.0 - r_out_perp * r_out_perp)) * n;
    refracted = Ray(hitInfo.point, r_out_perp + r_out_parallel);

    float t_s = (2 * cos_i) /
                (cos_i + refractionRatio * cos_t);
    float t_p = (2 * cos_i) /
                (refractionRatio * cos_i + cos_t);
    float k = refractionRatio * cos_t / cos_i;
    float T = 0.5f * (t_s * t_s + t_p * t_p) * k;

    attenuation = multElementByElement(Color(T, T, T), attenuation);
    return true;
}

Vec3f refractVec(const Vec3f& v, const Vec3f& n, float etai_over_etat) {
    auto cos_theta = fmin((-v * n) / v.norm(), 1.0f);
    Vec3f r_out_perp = etai_over_etat * (v + cos_theta * n);
    Vec3f r_out_parallel = -sqrt(fabs(1.0 - r_out_perp * r_out_perp)) * n;
    return r_out_perp + r_out_parallel;
}

#endif //__MATERIAL_FRENEL_H__