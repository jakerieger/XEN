//
// Created by jr on 2/12/24.
//

#pragma once

#include <stdint.h>

struct FSize {
    FSize() = default;
    FSize(const uint32_t width, const uint32_t height)
        : Width(width), Height(height) {}
    uint32_t Width;
    uint32_t Height;
};
