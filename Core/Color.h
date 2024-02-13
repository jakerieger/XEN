//
// Created by jr on 2/12/24.
//

#pragma once

#include "Utilities.inl"

struct FColor {
    explicit FColor(uint32_t color);
    explicit FColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    explicit FColor(uint8_t v);
    FColor(float r, float g, float b, float a = 1.0f);
    explicit FColor(float v);

    float Red;
    float Green;
    float Blue;
    float Alpha = 1.0f;
};
