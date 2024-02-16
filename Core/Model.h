//
// Created by jr on 2/15/24.
//

#pragma once

#include "Mesh.h"
#include "STL.h"

#include <Shader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class AModel {
public:
    explicit AModel(const char* path, AShader shader);

    void Draw();
    void Destroy();

    AShader& GetShader() {
        return m_Shader;
    }

private:
    vector<AMesh> m_Meshes;
    AShader m_Shader;

    void LoadModel(const string& path);
    void ProcessNode(const aiNode* node, const aiScene* scene);
    static AMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};