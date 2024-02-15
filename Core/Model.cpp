//
// Created by jr on 2/15/24.
//

#include "Model.h"

AModel::AModel(const char* path) {
    LoadModel(path);
}

void AModel::Draw(const AShader& shader) {
    for (auto& mesh : m_Meshes) {
        mesh.Draw(shader);
    }
}

void AModel::Destroy() {
    for (auto& mesh : m_Meshes) {
        mesh.Destroy();
    }
}

void AModel::LoadModel(const string& path) {
    Assimp::Importer import;
    const aiScene* scene =
      import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        fprintf(stderr, "ERROR::ASSIMP::%s\n", import.GetErrorString());
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}

void AModel::ProcessNode(aiNode* node, const aiScene* scene) {}

AMesh AModel::ProcessMesh(aiMesh* mesh, const aiScene* scene) {}