#ifndef COMPLEXOBJECTLOADER_H
#define COMPLEXOBJECTLOADER_H

#include "datastructures.h"

#include <vector>

class CObjectImp
{
    std::vector<MeshData*> meshes;
public:
    bool ReadObjectFromBinFile(const std::string& filename);
    std::vector<MeshData*>& GetMeshes();
    ~CObjectImp();
};

#endif
