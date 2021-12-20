#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "../utils/color.h"
#include "../utils/myMath.h"
#include "../utils/ray.h"
#include "../utils/hitInfo.h"

class Material {
public:
    virtual bool scatter(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& scattered) const { return false; };
    virtual bool reflect(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& reflected) const { return false; };
    virtual bool refract(const Ray& incommingRay, const HitInfo& hitInfo, Color& attenuation, Ray& refracted) const { return false; };
};

#endif //__MATERIAL_H__