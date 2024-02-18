//
// Created by jr on 2/14/24.
//

#pragma once

#include "STL.h"
#include "Scene.h"

namespace Profiler {
    extern string GpuVendor;
    extern string GpuRenderer;
    extern float TotalMemory;
    extern float FreeMemory;
    extern float UsedMemory;
    extern bool CanQueryMemory;

    void Initialize();
    void Start();
    void Update();
    void Shutdown();

    uint32_t GetTotalTriangles(AScene* scene);
}  // namespace Profiler