#pragma once

#include "pch/pch.h"
#include "Mesh.h"
#include "Debug.h"


class Model : public Mesh
{
public:
    Model(const std::string& path);

private:
    std::vector<Mesh> meshes;

private:
    void processNode(aiNode* node, const aiScene*);
    Mesh processMesh(aiMesh* node, const aiScene*);
};