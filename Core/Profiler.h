//
// Created by jr on 2/14/24.
//

#pragma once

#include "STL.h"

namespace Profiler {
    namespace GPU {
        string GetDeviceVendor();
        string GetDeviceRenderer();
        void GetMemoryUsage(float& totalMemory,
                            float& usedMemory,
                            float& freeMemory);
    }  // namespace GPU
}  // namespace Profiler