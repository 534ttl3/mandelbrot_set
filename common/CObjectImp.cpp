
#include "CObjectImp.h"
#include "datastructures.h"

#include <glm/glm.hpp>

#include <iostream>
#include <bitset>
#include <fstream>
using namespace std;

CObjectImp::~CObjectImp()
{
    for(unsigned int i=0; i<meshes.size(); i++)
        delete meshes[i];
}

std::vector<MeshData*>& CObjectImp::GetMeshes()
{
    return meshes;
}

bool CObjectImp::ReadObjectFromBinFile(const std::string& filename)
{
    cout << "Importing object " << filename << endl;
    ifstream bin_in(filename.c_str(), ios::in | ios::binary);
    if(!bin_in.good())
    {
        cout << "Error ReadObjectFromBinFile: invalid file " << filename << endl;
        return false;
    }
    // first read the number of meshes (uint32_t)
    uint32_t numofmeshes = 0;
    bin_in.read((char*)&numofmeshes, sizeof(uint32_t));
    if(!numofmeshes)
    {
        cout << "Error ReadFileFromBinFormat: numofmeshes=" << numofmeshes << endl;
        return false;
    }

    // next read all the individual meshes
    for(unsigned int i=0; i<numofmeshes; i++)
    {
        vector<VertexData> vertices;
        vector<unsigned int> indices;
        vector<TextureData*> textures;

        uint32_t id;
        bin_in.read((char*)&id, sizeof(uint32_t));
        uint32_t numofvertices = 0;
        bin_in.read((char*)&numofvertices, sizeof(uint32_t));
        if(!numofvertices)
        {
            cout << "Error ReadFileFromBinFormat: numofvertices=" << numofvertices << endl;
            return false;
        }
        uint32_t numofindices;
        bin_in.read((char*)&numofindices, sizeof(uint32_t));
        if(!numofindices)
        {
            cout << "Error ReadFileFromBinFormat: numofindices=" << numofindices << endl;
            return false;
        }

        uint32_t numoftextures;
        bin_in.read((char*)&numoftextures, sizeof(uint32_t));
        if(numoftextures > 1)
        {
            cout << "Error ReadFileFromBinFormat: numoftextures=" << numoftextures << endl;
            return false;
        }

        // next read material properties
        MaterialData material;
        unsigned char materialbitmask = MATERIAL_AMBIENT | MATERIAL_DIFFUSE | MATERIAL_SPECULAR
                                        | MATERIAL_SHININESS;
        bin_in.read((char*)&material.diffuse, sizeof(vec4f));
        bin_in.read((char*)&material.ambient, sizeof(vec4f));
        bin_in.read((char*)&material.specular, sizeof(vec4f));
        bin_in.read((char*)&material.shininess, sizeof(float));
        bin_in.read((char*)&material.emissive, sizeof(vec4f));

        // read vertex attribute attributebitmask
        unsigned char attributebitmask = 0;
        bin_in.read((char*)&attributebitmask, sizeof(unsigned char));
        if(!attributebitmask)
        {
            cout << "Error ReadFileFromBinFormat: reading the attributebitmask failed" << endl;
            return false;
        }

        for(unsigned int j=0; j<numofvertices; j++)
        {
            VertexData vertex;

            // fill rest with some data
            vec3f position;
            vec3f normal;
            float U = 0;
            float V = 0;
            vec3f tangent;
            vec3f bitangent;
            vec4f color = vec4f(1.0f, 1.0f, 1.0f, 1.0f);

            if(attributebitmask & A_POS)
            {
                bin_in.read((char*)&position, sizeof(vec3f));
            }
            if(attributebitmask & A_NOR)
            {
                bin_in.read((char*)&normal, sizeof(vec3f));
            }
            if(attributebitmask & A_UV)
            {
                bin_in.read((char*)&U, sizeof(float));
                bin_in.read((char*)&V, sizeof(float));
            }
            if(attributebitmask & A_TAN)
            {
                bin_in.read((char*)&tangent, sizeof(vec3f));
            }
            if(attributebitmask & A_BIT)
            {
                bin_in.read((char*)&bitangent, sizeof(vec3f));
            }
            if(attributebitmask & A_COL)
            {
                bin_in.read((char*)&color, sizeof(vec4f));
            }

            to_glm3f(vertex.position, position);
            to_glm3f(vertex.normal, normal);
            vertex.U = U;
            vertex.V = V;
            to_glm3f(vertex.tangent, tangent);
            to_glm3f(vertex.bitangent, bitangent);
            to_glm4f(vertex.color, color);

            vertices.push_back(vertex);
        }

        for(unsigned int i=0; i<numofindices; i++)
        {
            uint32_t index;
            bin_in.read((char*)&index, sizeof(uint32_t));
            indices.push_back(index);
        }

        for(unsigned int j=0; j<numoftextures; j++)
        {
            TextureData* ptexdata = new TextureData();

            bin_in.read((char*)&ptexdata->numofcolors, sizeof(uint32_t));

            bin_in.read((char*)&ptexdata->width,  sizeof(uint32_t));

            bin_in.read((char*)&ptexdata->height, sizeof(uint32_t));

            unsigned char* tmppixels = new unsigned char[ptexdata->numofcolors*
                    ptexdata->width*
                    ptexdata->height];
            bin_in.read((char*)tmppixels,
                        ptexdata->width*ptexdata->height*ptexdata->numofcolors*sizeof(unsigned char));

            ptexdata->AssignRawPixels(tmppixels);
            delete tmppixels;

            textures.push_back(ptexdata);
        }


        meshes.push_back(new MeshData(id, &vertices, &indices, &material, &textures));
    }

    bin_in.close();
    return true;
}
