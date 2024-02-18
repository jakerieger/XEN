//
// Created by jr on 2/14/24.
//

#include "Profiler.h"

#include "Drawable.h"
#include "MeshRenderer.h"

#include <glad/glad.h>

namespace Profiler {
    string GetDeviceVendor() {
        const unsigned char* vendor = glGetString(GL_VENDOR);
        return reinterpret_cast<const char*>(vendor);
    }

    string GetDeviceRenderer() {
        const unsigned char* renderer = glGetString(GL_RENDERER);
        return reinterpret_cast<const char*>(renderer);
    }

    void
    GetMemoryInfo(float& totalMemory, float& usedMemory, float& freeMemory) {
        glGetFloatv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX,
                    &totalMemory);
        glGetFloatv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
                    &freeMemory);
        usedMemory = totalMemory - freeMemory;
    }

    string GpuVendor;
    string GpuRenderer;
    float TotalMemory;
    float FreeMemory;
    float UsedMemory;
    bool CanQueryMemory = false;
    bool IsRunning      = false;

    void Initialize() {
        if (GetDeviceVendor() == "NVIDIA Corporation") {
            CanQueryMemory = true;
        }
    }

    void Start() {
        IsRunning   = true;
        GpuVendor   = GetDeviceVendor();
        GpuRenderer = GetDeviceRenderer();
    }

    void Update() {
        if (IsRunning && CanQueryMemory) {
            GetMemoryInfo(TotalMemory, UsedMemory, FreeMemory);
        }
    }

    void Shutdown() {
        IsRunning   = false;
        GpuVendor   = "";
        GpuRenderer = "";
        TotalMemory = 0.f;
        FreeMemory  = 0.f;
        UsedMemory  = 0.f;
    }

    uint32_t GetTotalTriangles(AScene* scene) {
        uint32_t totalTris = 0;

        for (const auto gameObjects = scene->GetContext().m_GameObjects;
             const auto go : gameObjects) {
            if (const auto drawable = dynamic_cast<IDrawable*>(go);
                drawable && drawable->GetMeshRenderer()) {
                for (auto& mesh : drawable->GetMeshRenderer()->GetMeshes()) {
                    totalTris += mesh.GetNumTriangles();
                }
            }
        }

        return totalTris;
    }
}  // namespace Profiler