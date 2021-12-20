#ifndef __MATERIAL_LAMBERTIAN_COLORED__
#define __MATERIAL_LAMBERTIAN_COLORED__

#include "material.h"
#include "../utils/myMath.h"

class MaterialLambertianColored : public Material {
public:
    MaterialLambertianColored(Color albedo) : Material(), _albedo(albedo) {};

    bool scatter(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const override;
private:
    Color _albedo;
};

bool MaterialLambertianColored::scatter(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const {
    Vec3f scatterDir = hitInfo.normal + randomInUnitSphere();
    if (closeToZero(scatterDir)) {
        scatterDir = hitInfo.normal;
    }
    scatterDir = scatterDir.normalize();

    scattered = Ray(hitInfo.point, scatterDir);
    attenuation = _albedo;
    return true;
}

#endif //__MATERIAL_LAMBERTIAN_COLORED__