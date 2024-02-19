//
// Created by conta on 2/18/2024.
//

#pragma once

#include "Interfaces/Component.h"
#include "Material.h"
#include "Mesh.h"
#include "STL.h"
#include "Transform.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

struct FSceneContext;

class AMeshRenderer final : public IComponent {
public:
    explicit
    AMeshRenderer(const char* path, IMaterial* material, ATransform* transform);

    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Draw(FSceneContext& sceneContext);
    void Destroyed(FSceneContext& sceneContext) override;

    vector<AMesh>& GetMeshes() {
        return m_Meshes;
    }

private:
    vector<AMesh> m_Meshes;
    IMaterial* m_Material;
    ATransform* m_Transform;

private:
    void LoadModel(const string& path);
    void ProcessNode(const aiNode* node, const aiScene* scene);
};