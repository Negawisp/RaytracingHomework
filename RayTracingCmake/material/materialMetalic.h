#ifndef __MATERIAL_METALIC__
#define __MATERIAL_METALIC__

#include "material.h"
#include "../utils/myMath.h"

class MaterialMetalic : public Material {
public:
    MaterialMetalic(Color albedo) : Material(), _albedo(albedo) {};

    bool reflect(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const override;
private:
    Color _albedo;
};

bool MaterialMetalic::reflect(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& reflected) const {
    Vec3f reflectedDir = reflectVec(incommingRay.getDirection(), hitInfo.normal);
    reflected = Ray(hitInfo.point, reflectedDir);
    attenuation = _albedo;
    return reflectedDir * hitInfo.normal > 0;
}

#endif //__MATERIAL_METALIC__