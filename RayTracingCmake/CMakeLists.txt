# CMakeList.txt : CMake project for RayTracingCmake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories(${CMAKE_SOURCE_DIR}/external)
include_directories(${CMAKE_SOURCE_DIR}/external/vkutils)
include_directories(${CMAKE_SOURCE_DIR}/external/vkutils/geom)
include_directories(${CMAKE_SOURCE_DIR}/external/volk)
include_directories(${CMAKE_SOURCE_DIR}/external/eigen)

# Add source to this project's executable.
add_executable (RayTracingCmake "main.cpp" "../external/vkutils/geom/cmesh.cpp" "utils/myMath.cpp" "utils/vec.h" "utils/stb_image_write.h" "hittable/hittable.h" "hittable/sphereHittable.h"   "light/pointLightSource.h" "utils/color.h" "utils/ray.h" "hittable/hittableList.h" "material/material.h" "utils/hitInfo.h" "material/materialMetalic.h" "mesh/mesh.h" "utils/matrix.h" "hittable/boxHittable.h" "utils/aabb.h" "hittable/xyRectHittable.h" "hittable/zxRectHittable.h" "hittable/yzRectHittable.h" "hittable/instance.h" "mesh/sceneLoader.h" "mesh/loader_utils/hydraxml.cpp" "mesh/loader_utils/hydraxml.h" "mesh/loader_utils/images.cpp" "mesh/loader_utils/images.h" "mesh/loader_utils/pugiconfig.hpp" "mesh/loader_utils/pugixml.cpp" "mesh/loader_utils/pugixml.hpp" "utils/logging.h" "utils/linal.h" "utils/aabb.cpp"   "material/materialSimpleRefractor.h" "material/materialPureFrenel.h")

# TODO: Add tests and install targets if needed.
