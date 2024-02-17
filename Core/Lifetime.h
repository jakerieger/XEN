//
// Created by conta on 2/16/2024.
//

#pragma once

class ILifetime {
public:
    virtual ~ILifetime()        = default;
    virtual void Awake()        = 0;
    virtual void Start()        = 0;
    virtual void Update()       = 0;
    virtual void LateUpdate()   = 0;
    virtual void FixedUpdated() = 0;
    virtual void Destroyed()    = 0;
};