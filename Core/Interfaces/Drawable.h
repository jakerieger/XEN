//
// Created by conta on 2/18/2024.
//

#pragma once

class AMeshRenderer;
struct FSceneContext;

class IDrawable {
public:
    virtual ~IDrawable()                           = default;
    virtual void Draw(FSceneContext& sceneContext) = 0;

    void SetMeshRenderer(AMeshRenderer* mesh) {
        m_MeshRenderer = mesh;
    }

    AMeshRenderer* GetMeshRenderer() const {
        return m_MeshRenderer;
    }

protected:
    AMeshRenderer* m_MeshRenderer = nullptr;
};