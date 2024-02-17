//
// Created by conta on 2/16/2024.
//

#pragma once
#include "Lifetime.h"

class AGameObject final : public ILifetime {
public:
    void Awake() override;
    void Start() override;
    void Update() override;
    void LateUpdate() override;
    void FixedUpdated() override;
    void Destroyed() override;
};