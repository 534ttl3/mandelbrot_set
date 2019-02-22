#include "datastructures.h"

#include <glm/glm.hpp>

unsigned int get_pos_offset()
{
    return 0;
}
unsigned int get_nor_offset()
{
    return sizeof(float)*3;
}
unsigned int get_uv_offset()
{
    return 2*sizeof(float)*3;
}
unsigned int get_tan_offset()
{
    return 2*sizeof(float)*3 + 2*sizeof(float);
}
unsigned int get_bit_offset()
{
    return 3*sizeof(float)*3 + 2*sizeof(float);
}
unsigned int get_col_offset()
{
    return 4*sizeof(float)*3 + 2*sizeof(float);
}

unsigned char GetLightTypeByString(const std::string& str)
{
    if(str == "POINTLIGHT")
        return LIGHT_POINT;
    else if(str == "DIRECTIONALLIGHT")
        return LIGHT_DIRECTIONAL;
    else if(str == "SPOTLIGHT")
        return LIGHT_SPOT;
    else
        return 0;
}

void to_glm3f(glm::vec3& _glm, vec3f& vec)
{
    _glm.x = vec.x;
    _glm.y = vec.y;
    _glm.z = vec.z;
}

void to_glm4f(glm::vec4& _glm, vec4f& vec)
{
    _glm.x = vec.x;
    _glm.y = vec.y;
    _glm.z = vec.z;
    _glm.w = vec.w;
}

void CalcBoundingSphere(BoundingSphere& sphere, std::vector<VertexData>& vertices)
{
    int n;
    float dx, dy, dz, le;

    n = vertices.size();
    //Bestimmung des Schwerpunktes

    sphere.center.x = 0;
    sphere.center.y = 0;
    sphere.center.z = 0;

    for(int i=0; i<n; i++)
    {
        sphere.center.x = sphere.center.x + vertices[i].position.x;
        sphere.center.y = sphere.center.y + vertices[i].position.y;
        sphere.center.z = sphere.center.z + vertices[i].position.z;
    }

    sphere.center.x = sphere.center.x / n;
    sphere.center.y = sphere.center.y / n;
    sphere.center.z = sphere.center.z / n;

    //längstmöglichsten Abstand vom Schwerpunkt aus ermitteln

    sphere.radius = 0;

    for(int i=0; i<vertices.size(); i++)
    {
        dx = vertices[i].position.x - sphere.center.x;
        dy = vertices[i].position.y - sphere.center.y;
        dz = vertices[i].position.z - sphere.center.z;
        le = sqrt(dx*dx+dy*dy+dz*dz);

        if(sphere.radius < le)
            sphere.radius = le;
    }
}

