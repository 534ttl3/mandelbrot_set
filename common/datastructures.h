#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H


#include <glm/glm.hpp>

#include <iostream>
#include <stdlib.h>
#include <map>
#include <vector>
#include <cstdint>
#include <string>

using namespace std;

struct render_props
{
    render_props() { }
    render_props(unsigned char attributes, unsigned char matrices, unsigned int light, unsigned int material)
    {
        p[0] = attributes;
        p[1] = matrices;
        p[2] = light;
        p[3] = material;
    }
    unsigned char p[4];
};

struct shaderstruct
{
    unsigned int programid;
    unsigned char bitmask[4];
};

// potential attributes
const unsigned char A_POS = 0b00000001;
const unsigned char A_NOR = 0b00000010;
const unsigned char A_UV  = 0b00000100;
const unsigned char A_TAN = 0b00001000;
const unsigned char A_BIT = 0b00010000;
const unsigned char A_COL = 0b00100000;

// potential matrices to be passed to shader
const unsigned char MATRIX_M   = 0b00000001;
const unsigned char MATRIX_V   = 0b00000010;
const unsigned char MATRIX_P   = 0b00000100;
const unsigned char MATRIX_N   = 0b00001000;
const unsigned char MATRIX_MVP = 0b00010000;

// potential lights
const unsigned char LIGHT_POINT       = 0b00000001;
const unsigned char LIGHT_DIRECTIONAL = 0b00000010;
const unsigned char LIGHT_SPOT        = 0b00000100;

// potential materials
const unsigned char MATERIAL_AMBIENT   = 0b00000001;
const unsigned char MATERIAL_DIFFUSE   = 0b00000010;
const unsigned char MATERIAL_SPECULAR  = 0b00000100;
const unsigned char MATERIAL_SHININESS = 0b00001000;


// for shader selection
const unsigned char POS                    = A_POS;
const unsigned char POS_COL                = A_POS |                                A_COL;
const unsigned char POS_NOR                = A_POS | A_NOR;
const unsigned char POS_NOR_COL            = A_POS | A_NOR |                        A_COL;
const unsigned char POS_NOR_UV             = A_POS | A_NOR | A_UV;
const unsigned char POS_NOR_UV_TAN_BIT     = A_POS | A_NOR | A_UV | A_TAN | A_BIT;
const unsigned char POS_NOR_UV_TAN_BIT_COL = A_POS | A_NOR | A_UV | A_TAN | A_BIT | A_COL;


unsigned int get_pos_offset();
unsigned int get_nor_offset();
unsigned int get_uv_offset();
unsigned int get_tan_offset();
unsigned int get_bit_offset();
unsigned int get_col_offset();

unsigned char GetLightTypeByString(const std::string& str);

struct vec3f
{
    vec3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
    vec3f() { }
    float x, y, z;
};

struct vec4f
{
    vec4f(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }
    vec4f() { }
    float x, y, z, w;
};

struct vec2f
{
    vec2f(float _x, float _y) : x(_x), y(_y) { }
    vec2f() { }
    float x, y;
};


void to_glm3f(glm::vec3& _glm, vec3f& vec);
void to_glm4f(glm::vec4& _glm, vec4f& vec);


// light sources
struct LightData
{
    unsigned int index;
    unsigned char type;
    glm::vec3 position;
    glm::vec3 intensity;
    glm::vec3 direction;
    float exponent;
    float cutoff;
};

struct MaterialData
{
    glm::vec4 diffuse;
    glm::vec4 ambient;
    glm::vec4 specular;
    float shininess;
    glm::vec4 emissive;
};

struct VertexData
{
    glm::vec3 position;
    glm::vec3 normal;
    float U, V;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec4 color;
};

struct TextureData
{
    TextureData()
    {
        numofcolors = 0;
        width = 0;
        height = 0;
        rawpixels = nullptr;
    }

    // call this after numofcolors, width and height are set
    void AssignRawPixels(unsigned char* _pixels)
    {
        if(numofcolors==0 || height==0 || width==0)
        {
            cout << "Error AssignRawPixels(): "
                 << " numofcolors=" << numofcolors
                 << " height=" << height
                 << " width=" << width << endl;
        }

        rawpixels = new unsigned char[width*height*numofcolors];

        for(unsigned int i=0; i<width*height*numofcolors; i++)
            rawpixels[i] = _pixels[i];
    }

    ~TextureData()
    {
        delete rawpixels;
    }

    uint32_t numofcolors;
    uint32_t width;
    uint32_t height;
    unsigned char* rawpixels;
};

struct BoundingSphere
{
    glm::vec3 center;
    float radius;
};


void CalcBoundingSphere(BoundingSphere& sphere, std::vector<VertexData>& vertices);

struct MeshData
{
    MeshData(unsigned int _id, std::vector<VertexData>* vertexdata,
             std::vector<unsigned int>* indexdata,
             MaterialData* _materialdata=nullptr, std::vector<TextureData*>* texturedata=nullptr)
    {
        id = _id;
        vertices=*vertexdata;
        indices=*indexdata;
        if(texturedata)
            textures=*texturedata;
        material = *_materialdata;

        cout << "number of vertices in mesh " << id << ": " << vertices.size() << endl;
        cout << "number of indices  in mesh " << id << ": " << indices.size() << endl;
        cout << "number of textures in mesh " << id << ": " << textures.size() << endl;

        CalcBoundingSphere(boundingsphere, vertices);

        cout.precision(2);
        cout << fixed;
        cout << "Bounding Sphere: center: (" << boundingsphere.center.x << " | "
             << boundingsphere.center.y << " | "
             << boundingsphere.center.z << " )"
             << ", radius: " << boundingsphere.radius << endl;
    }

    ~MeshData()
    {
        for(unsigned int i=0; i<textures.size(); i++)
            delete textures[i];
    }

    MaterialData material;
    BoundingSphere boundingsphere;
    unsigned int id;
    std::vector<VertexData> vertices;
    std::vector<TextureData*> textures;
    std::vector<unsigned int> indices;
};

#endif // DATASTRUCTURES_H
