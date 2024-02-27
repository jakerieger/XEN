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
class ACamera;
class AShader;

enum class EDrawPass : u8 {
    PASS_DEPTH,
    PASS_MAIN,
};

class AMeshRenderer final : public IComponent {
public:
    explicit AMeshRenderer(const char* path,
                           eastl::unique_ptr<IMaterial>& material);

    void Start(FSceneContext& sceneContext) override;
    void Update(float deltaTime, FSceneContext& sceneContext) override;
    void Draw(FSceneContext& sceneContext,
              const ATransform* transform,
              EDrawPass drawPass);
    void Destroyed(FSceneContext& sceneContext) override;

    eastl::vector<AMesh>& GetMeshes() {
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
    eastl::vector<AMesh> m_Meshes;
    eastl::unique_ptr<IMaterial> m_Material;
    eastl::unique_ptr<AShader> m_DepthShader;
    bool m_ReceivesShadows = true;

private:
    void LoadModel(const eastl::string& path);
    void ProcessNode(const aiNode* node, const aiScene* scene);
};