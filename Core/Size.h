//
// Created by jr on 2/12/24.
//

#pragma once

#include <stdint.h>

struct FSize {
    FSize(const int& width, const int& height) {
        this->Width  = width;
        this->Height = height;
    }
    FSize() = default;
    uint16_t Width;
    uint16_t Height;
};
