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
    explicit IGameObject(const string& name)
        : m_Name(name), m_Transform(ATransform()) {}

    string GetName() const {
        return m_Name;
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

    template<typename T>
    T* Cast() {
        return dynamic_cast<T*>(this);
    }

protected:
    string m_Name;
    ATransform m_Transform;
};