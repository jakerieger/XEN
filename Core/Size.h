//
// Created by jr on 2/12/24.
//

#pragma once

#include "Types.h"

struct FSize {
    FSize() = default;
    FSize(const u32 width, const u32 height) : Width(width), Height(height) {}
    u32 Width;
    u32 Height;
};
