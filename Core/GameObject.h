//
// Created by conta on 2/16/2024.
//

#pragma once
#include "Component.h"
#include "Lifetime.h"
#include "STL.h"
#include "Transform.h"

struct FSceneContext;

class IGameObject : public ILifetime {
public:
    explicit IGameObject(const uint32_t id)
        : m_ID(id), m_Transform(ATransform()) {}

    uint32_t GetID() const {
        return m_ID;
    }

    virtual void Awake(FSceneContext& sceneContext) {
        m_Transform.Awake(sceneContext);
    }

    virtual void Start(FSceneContext& sceneContext) {
        m_Transform.Start(sceneContext);
    }

    virtual void Update(const float deltaTime, FSceneContext& sceneContext) {
        m_Transform.Update(deltaTime, sceneContext);
    }

    virtual void LateUpdate(FSceneContext& sceneContext) {
        m_Transform.LateUpdate(sceneContext);
    }

    virtual void FixedUpdated(FSceneContext& sceneContext) {
        m_Transform.FixedUpdated(sceneContext);
    }

    virtual void Destroyed(FSceneContext& sceneContext) {
        m_Transform.Destroyed(sceneContext);
    }

    ATransform* GetTransform() {
        return &m_Transform;
    }

protected:
    uint32_t m_ID;
    ATransform m_Transform;
};