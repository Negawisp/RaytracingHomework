#ifndef __MESH_H__
#define __MESH_H__

#include "../utils/myMath.h"

class Mesh {
public:
    Mesh();


private:
    std::vector<Point3> _vertexBuffer;
    std::vector<Vec3i>  _indexBuffer;
};

#endif //__MESH_H__