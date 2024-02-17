//
// Created by jr on 2/15/24.
//

#pragma once

#include "Material.h"
#include "Mesh.h"
#include "STL.h"
#include "Transform.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

struct FSceneContext;
class AModel {
public:
    explicit AModel(const char* path, IMaterial* material);

    void Update(float deltaTime);
    void Draw(FSceneContext& sceneContext);
    void Destroy();

    ATransform& GetTransform() {
        return m_Transform;
    }

private:
    vector<AMesh> m_Meshes;
    IMaterial* m_Material;
    ATransform m_Transform;

    void LoadModel(const string& path);
    void ProcessNode(const aiNode* node, const aiScene* scene);
    static AMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};