#ifndef __POINT_LIGHT_SOURCE_H__
#define __POINT_LIGHT_SOURCE_H__

#include "../utils/vec.h"

class PointLightSource {
public:
    PointLightSource(Vec3f position, Color color) :
        _position(position), _color(color) {}

    ~PointLightSource(){}

    Vec3f getPosition()  { return _position; }
    Color getColor() { return _color; }

private:
    Vec3f _position;
    Color _color;
};

#endif //__POINT_LIGHT_SOURCE_H__