#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "utils/myMath.h"
#include "utils/matrix.h"
#include "utils/color.h"
#include "utils/stb_image_write.h"
#include "hittable/hittableList.h"
#include "hittable/sphereHittable.h"
#include "hittable/boxHittable.h"
#include "hittable/instance.h"
#include "hittable/meshHittable.h"
#include "material/materialLambertianColored.h"
#include "material/materialMetalic.h"
#include "material/materialSimpleRefractor.h"
#include "material/materialPureFrenel.h"
#include "light/pointLightSource.h"
#include "mesh/sceneLoader.h"

typedef std::vector<shared_ptr<PointLightSource>> PointLights;


const Vec3f CAM_POS(0, 0, 1.2f);
const float CAM_PLANE_HEIGHT = 0.96f;
const float CAM_PLANE_WIDTH = 1.28f;
const float CAM_PLANE_DIST = -1.f;

const float HIT_TOLERANCE = 0.001f;
const unsigned int REFLECTIONS_DEPTH = 6;
const unsigned int DIFFUSION_SOURCES_COUNT = 10;


int ScreenWidth = 256;
int ScreenHeight = 192;

float AmbientOcclusionRadius = 50.f;
float DirectLightImpact = 0.01f;
float EnvironmentLightImpact = 0.99f;


Color environmentalLight(const Ray& ray) {
    Vec3f unitDirection = ray.getDirection().normalize();
    auto t = 0.5 * (unitDirection.y + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

Color castRay(const Ray& ray, HittableList& hittables, PointLights& pointLights, unsigned int depth) {
    HitInfo hit = {};
    HitInfo hitTemp = {};

    Color retColor(0.f, 0.f, 0.f);
    if (depth <= 0) {
        return retColor;
    }

    AABB boundingBox;
    if (!hittables.getBoundingBox(boundingBox) ||
        !boundingBox.rayIntersects(ray, HIT_TOLERANCE, positiveInfinity) ||
        !hittables.rayIntersects(ray, HIT_TOLERANCE, positiveInfinity, hit))
    {
        // Background
        return environmentalLight(ray) * EnvironmentLightImpact;
    }

    // Point light sources
    Color directLight(0.f, 0.f, 0.f);
    {
        Vec3f dir(0.f, 0.f, 0.f);
        float t = 0;
        for each (shared_ptr<PointLightSource> light in pointLights)
        {
            dir = light->getPosition() - hit.point;
            t = dir.norm();
            dir = dir.normalize();
            if (hittables.rayIntersects(Ray(hit.point, dir), HIT_TOLERANCE, t, hitTemp)) {
                continue;
            }
            directLight += light->getColor() * (1 / (t * t)) * fmax((hit.normal * dir), 0.f);
        }
        for (int i = 0; i < 3; i++) {
            if (directLight[i] > 1.f) {
                directLight[i] = 1.f;
            }
        }
        directLight = directLight * DirectLightImpact;
    }

    // Diffusion light (ambient occlusion)
    Color diffusionLight(0.f, 0.f, 0.f);
    Color attenuation(0.f, 0.f, 0.f);
    {
        Ray scattered;
        for (int i = 0;
            i < DIFFUSION_SOURCES_COUNT && hit.material->scatter(ray, hit, attenuation, scattered);
            i++)
        {
            if (!hittables.rayIntersects(scattered, HIT_TOLERANCE, AmbientOcclusionRadius, hitTemp)) {
                diffusionLight += environmentalLight(scattered);
            }
        }
        diffusionLight = diffusionLight * (1.f / (float)DIFFUSION_SOURCES_COUNT) * EnvironmentLightImpact;
    }
    retColor += multElementByElement(attenuation, diffusionLight + directLight);

    // Reflection
    Color reflectionColor;
    {
        Ray reflected;
        if (hit.material->reflect(ray, hit, attenuation, reflected)) {
            reflectionColor = castRay(reflected, hittables, pointLights, depth - 1);
        }
    }
    retColor += multElementByElement(attenuation, reflectionColor);

    // Refraction
    Color refractionColor;
    {
        Ray refracted;
        if (hit.material->refract(ray, hit, attenuation, refracted)) {
            refractionColor = castRay(refracted, hittables, pointLights, depth - 1);
        }
    }
    retColor += multElementByElement(attenuation, refractionColor);

    return retColor;
}


void render(const char* filename, HittableList& hittables, PointLights& lights) {
    std::vector<Vec3c> framebuffer(ScreenWidth * ScreenHeight);

    for (size_t j = 0; j < ScreenHeight; j++) {
        //std::cerr << "\rDrawing image. Progress: " << j * 100 / (SCREEN_HEIGHT - 1) << "%..." << std::flush;
        for (size_t i = 0; i < ScreenWidth; i++) {
            std::cerr << "\rDrawing image. Progress: " << j * 100 / (ScreenHeight - 1) << "." << i * 100 / (ScreenWidth - 1) << "%..." << std::flush;
            Vec3f dir = Vec3f(
                -(i / float(ScreenWidth)  - 0.5f) * CAM_PLANE_WIDTH,
                -(j / float(ScreenHeight) - 0.5f) * CAM_PLANE_HEIGHT,
                CAM_PLANE_DIST)
                .normalize();
            Color color = castRay(Ray(CAM_POS, dir), hittables, lights, REFLECTIONS_DEPTH);
            write_color(framebuffer[i + j * ScreenWidth], color);
        }
    }

    std::cerr << "\rImage drawn. Saving image... " << std::flush;
    assert(stbiWriteBmp(filename, ScreenWidth, ScreenHeight, 3, framebuffer.data()));
    std::cerr << "\rImage drawn and saved to " << filename << std::flush;
}

int main() {
    const char* filename = "out.bmp";

    int sceneNumber = 0;
    char input[30];
    {
        std::cout << "Scene selection.\nHere are 4 scenes:\n" <<
            "1. Cornell box (scene of meshes only)\n" <<
            "2. Teapot (takes ages to render)\n" <<
            "3. Spheres (scene with parallelogramm mesh and two spheres)\n" <<
            "Please, enter a scene number to select a scene to render: ";
        std::cin >> input;
        sceneNumber = atoi(input);
        while (sceneNumber < 1 || sceneNumber > 3) {
            std::cout << "Bad entry. Please, enter a scene number: ";
            std::cin >> input;
            sceneNumber = atoi(input);
        }
        std::cout << "Scene selected.\n";
    }

    int resolutionNumber = 0;
    {
        std::cout << "\nResolution selection.\nHere are 4 options:\n" <<
            "1. 1024x768\n" <<
            "2. 512x384\n" <<
            "3. 256x192\n" <<
            "4. 64x48\n" <<
            "Please, enter a resolution number: ";
        std::cin >> input;
        resolutionNumber = atoi(input);
        while (resolutionNumber < 1 || resolutionNumber > 4) {
            std::cout << "Bad entry. Please, enter a scene number: ";
            std::cin >> input;
            resolutionNumber = atoi(input);
        }
        std::cout << "Scene selected.\n";

        switch (resolutionNumber)
        {
        case 1: {
            ScreenWidth = 1024;
            ScreenHeight = 768;
        } break;
        case 2: {
            ScreenWidth = 512;
            ScreenHeight = 384;
        } break;
        case 3: {
            ScreenWidth = 256;
            ScreenHeight = 192;
        } break;
        case 4: {
            ScreenWidth = 64;
            ScreenHeight = 48;
        } break;
        default: {
            ScreenWidth = 256;
            ScreenHeight = 192;
        } break;
        }
    }

    SceneMeshes meshes;
    meshes.LoadSceneXML("../../../../resources/scenes/043_cornell_normals/statex_00001.xml");

    HittableList hittables;
    PointLights lights;
    switch (sceneNumber) {
    case 1:
    {
        AmbientOcclusionRadius = 1.f;
        DirectLightImpact = 0.8f;
        EnvironmentLightImpact = 0.2f;

        float k = 1.f / (2.f * 0.381f);

        shared_ptr<Material> lambertRed = make_shared<MaterialLambertianColored>(Color(1.f, 0.f, 0.f));
        shared_ptr<Material> lambertGreen = make_shared<MaterialLambertianColored>(Color(0.f, 1.f, 0.f));
        shared_ptr<Material> lambertYellow = make_shared<MaterialLambertianColored>(Color(1.f, 1.f, 0.f));
        shared_ptr<Material> lambertWhite = make_shared<MaterialLambertianColored>(Color(1.f, 1.f, 1.f));
        shared_ptr<Material> mirror = make_shared<MaterialMetalic>(Color(1.f, 1.f, 1.f));
        shared_ptr<Material> frenelGlass = make_shared<MaterialPureFrenel>(1.4f, Color(1.f, 1.f, 1.f));

        shared_ptr<Hittable> meshCilinder = make_shared<MeshHittable>(meshes.getMesh(3), lambertRed);
        shared_ptr<Hittable> meshAngle = make_shared<MeshHittable>(meshes.getMesh(4), lambertRed);
        shared_ptr<Hittable> meshPlane = make_shared<MeshHittable>(meshes.getMesh(5), lambertRed);


        hittables.add(make_shared<Instance>(lambertGreen, meshPlane, Vec3f(0.0f, 0.49f, 0.0f), k, Vec3f(0.f, 1.f, 0.f), 0));
        hittables.add(make_shared<Instance>(lambertRed, meshPlane, Vec3f(0.0f, -0.49f, 0.0f), k, Vec3f(0.f, 1.f, 0.f), 0));
        hittables.add(make_shared<Instance>(lambertWhite, meshPlane, Vec3f(0.49f, 0.0f, 0.0f), k, Vec3f(0.f, 0.f, 1.f), pi / 2));
        hittables.add(make_shared<Instance>(lambertWhite, meshPlane, Vec3f(-0.49f, 0.0f, 0.0f), k, Vec3f(0.f, 0.f, 1.f), pi / 2));
        hittables.add(make_shared<Instance>(mirror, meshPlane, Vec3f(0.0f, 0.0f, -0.49f), k, Vec3f(1.f, 0.f, 0.f), pi / 2));
        hittables.add(make_shared<Instance>(frenelGlass, meshCilinder, Vec3f(-0.2f, -0.49f, 0.0f), 1.f / 1.f, Vec3f(1.f, 0.f, 0.f), 0.f));
        hittables.add(make_shared<Instance>(lambertYellow, meshAngle, Vec3f(0.3f, -0.49f, -0.2f), 1.f / 2.f, Vec3f(0.f, 1.f, 0.f), pi / 6.f));

        lights.push_back(make_shared<PointLightSource>(Point3(0.f, 0.45f, 0.f), Color(0.25f, 0.25f, 0.25f)));
    } break;
    case 2: {
        AmbientOcclusionRadius = 1.f;
        DirectLightImpact = 0.2f;
        EnvironmentLightImpact = 0.8f;


        shared_ptr<Material> lambertRed = make_shared<MaterialLambertianColored>(Color(1.f, 0.f, 0.f));
        shared_ptr<Material> lambertWhite = make_shared<MaterialLambertianColored>(Color(1.f, 1.f, 1.f));
        shared_ptr<Material> mirror = make_shared<MaterialMetalic>(Color(1.f, 1.f, 1.f));
        shared_ptr<Material> frenelGlass = make_shared<MaterialPureFrenel>(1.4f, Color(1.f, 1.f, 1.f));

        shared_ptr<Hittable> meshTeapot = make_shared<MeshHittable>(meshes.getMesh(1), lambertRed);
        shared_ptr<Hittable> meshPlane = make_shared<MeshHittable>(meshes.getMesh(5), lambertRed);


        hittables.add(make_shared<Instance>(lambertWhite, meshPlane, Vec3f(0.0f, -0.49f, 0.0f), 1000.f, Vec3f(0.f, 1.f, 0.f), 0));
        hittables.add(make_shared<Instance>(frenelGlass, meshTeapot, Vec3f(0.0f, -0.49f, 0.0f), 0.3f, Vec3f(0.f, 1.f, 0.f), 0));
        hittables.add(make_shared<SphereHittable>(mirror, Vec3f(0.0f, 0.35f, -0.2f), 0.2f));

        lights.push_back(make_shared<PointLightSource>(Point3(0.4f, 0.75f, -0.2f), Color(0.27f, 0.5f, 0.27f)));
    } break;
    case 3: {
        AmbientOcclusionRadius = 0.5f;
        DirectLightImpact = 0.3f;
        EnvironmentLightImpact = 0.7f;


        shared_ptr<Material> lambertRed = make_shared<MaterialLambertianColored>(Color(1.f, 0.f, 0.f));
        shared_ptr<Material> lambertGreen = make_shared<MaterialLambertianColored>(Color(0.f, 1.f, 0.f));
        shared_ptr<Material> lambertYellow = make_shared<MaterialLambertianColored>(Color(1.f, 1.f, 0.f));
        shared_ptr<Material> lambertWhite = make_shared<MaterialLambertianColored>(Color(1.f, 1.f, 1.f));
        shared_ptr<Material> mirror = make_shared<MaterialMetalic>(Color(1.f, 1.f, 1.f));
        shared_ptr<Material> frenelGlass = make_shared<MaterialPureFrenel>(1.4f, Color(1.f, 1.f, 1.f));
        shared_ptr<Material> frenelDielectric = make_shared<MaterialPureFrenel>(6.f, Color(1.f, 0.7f, 0.2f));

        shared_ptr<Hittable> meshBox = make_shared<MeshHittable>(meshes.getMesh(2), lambertRed);
        shared_ptr<Hittable> meshPlane = make_shared<MeshHittable>(meshes.getMesh(5), lambertRed);


        hittables.add(make_shared<Instance>(lambertWhite, meshPlane, Vec3f(0.0f, -0.49f, 0.0f), 1000.f, Vec3f(0.f, 1.f, 0.f), 0));
        hittables.add(make_shared<SphereHittable>(mirror, Vec3f(0.0f, 0.35f, -0.2f), 0.2f));
        hittables.add(make_shared<SphereHittable>(lambertYellow, Vec3f(0.0f, -0.25f, 0.0f), 0.25f));
        hittables.add(make_shared<Instance>(frenelDielectric, meshBox, Vec3f(0.5f, -0.5f, -0.0f), 0.7f, Vec3f(0.0f, 1.0f, 0.0f), pi / 10.f));

        lights.push_back(make_shared<PointLightSource>(Point3(0.4f, 0.75f, -0.2f), Color(0.27f, 0.5f, 0.27f)));
    } break;
    default: {} break;
    }

    render(filename, hittables, lights);
    system(filename);
    return 0;
}