//
// Created by jr on 2/15/24.
//

#pragma once

#include "Mesh.h"
#include "STL.h"

#include <Shader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class AModel {
public:
    AModel(const char* path);

    void Draw(const AShader& shader);
    void Destroy();

private:
    vector<AMesh> m_Meshes;

    void LoadModel(const string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    AMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};