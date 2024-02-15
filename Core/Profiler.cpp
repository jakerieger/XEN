//
// Created by jr on 2/14/24.
//

#include "Profiler.h"
#include <glad/glad.h>

namespace Profiler {
    string GPU::GetDeviceVendor() {
        const unsigned char* vendor = glGetString(GL_VENDOR);
        return reinterpret_cast<const char*>(vendor);
    }

    string GPU::GetDeviceRenderer() {
        const unsigned char* renderer = glGetString(GL_RENDERER);
        return reinterpret_cast<const char*>(renderer);
    }

    void GPU::GetMemoryUsage(float& totalMemory,
                             float& usedMemory,
                             float& freeMemory) {
        glGetFloatv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX,
                    &totalMemory);
        glGetFloatv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
                    &freeMemory);
        usedMemory = totalMemory - freeMemory;
    }
}  // namespace Profiler