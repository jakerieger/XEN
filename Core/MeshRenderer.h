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
    explicit AMeshRenderer(const char* path, unique_ptr<IMaterial>& material);

    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Draw(FSceneContext& sceneContext, const ATransform* transform);
    void Destroyed(FSceneContext& sceneContext) override;

    vector<AMesh>& GetMeshes() {
        return m_Meshes;
    }

    IMaterial* GetMaterial() const {
        return m_Material.get();
    }

    template<typename T>
    T* GetMaterialAs() {
        return m_Material->Cast<T>();
    }

private:
    vector<AMesh> m_Meshes;
    unique_ptr<IMaterial> m_Material;

private:
    void LoadModel(const string& path);
    void ProcessNode(const aiNode* node, const aiScene* scene);
};