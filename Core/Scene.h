//
// Created by jr on 2/16/24.
//

#pragma once

#include "SceneContext.h"
#include "Camera.h"
#include "STL.h"

class AScene final : public ILifetime {
public:
    explicit AScene(const string& name) : m_Name(name) {}

    void Awake() override;
    void Start() override;
    void Update(float deltaTime) override;
    void LateUpdate() override;
    void FixedUpdated() override;
    void Destroyed() override;

    void Render();

    static ACamera* GetActiveCamera(FSceneContext& context);

    void SetActive(const bool active) {
        m_Active = active;
    }

    bool GetActive() const {
        return m_Active;
    }

    string& GetName() {
        return m_Name;
    }

    ADirectionalLight& GetSun() {
        return m_SceneContext.m_Sun;
    }

    FSceneContext& GetContext() {
        return m_SceneContext;
    }

    template<typename T>
    static vector<T*> FindAllGameObjectsOf(FSceneContext& context) {
        static_assert(std::is_base_of_v<IGameObject, T>,
                      "T must be a subclass of IGameObject");
        vector<T*> found;
        for (auto& go : context.m_GameObjects) {
            if (auto casted = dynamic_cast<T*>(go.get()); casted) {
                found.push_back(casted);
            }
        }

        return found;
    }

    template<typename T>
    static T* FindGameObjectOf(FSceneContext& context, const string& name) {
        static_assert(std::is_base_of_v<IGameObject, T>,
                      "T must be a subclass of IGameObject");
        for (auto& go : context.m_GameObjects) {
            if (auto casted = dynamic_cast<T*>(go.get());
                casted && casted->GetName() == name) {
                return casted;
            }
        }

        return nullptr;
    }

    static IGameObject* FindGameObject(FSceneContext& context,
                                       const string& name);

    template<typename T>
    void AddGameObject(unique_ptr<T>& gameObject) {
        static_assert(std::is_base_of_v<IGameObject, T>,
                      "T must be a subclass of IGameObject");
        m_SceneContext.m_GameObjects.push_back(move(gameObject));
    }

private:
    FSceneContext m_SceneContext;
    string m_Name;
    bool m_Active = false;
};