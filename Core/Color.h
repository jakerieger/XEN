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

namespace Colors {
    inline const FColor White   = FColor(0xFFFFFFFF);
    inline const FColor Black   = FColor(0xFF000000);
    inline const FColor Red     = FColor(0xFFFF0000);
    inline const FColor Yellow  = FColor(0xFFFFFF00);
    inline const FColor Green   = FColor(0xFF00FF00);
    inline const FColor Cyan    = FColor(0xFF00FFFF);
    inline const FColor Blue    = FColor(0xFF0000FF);
    inline const FColor Magenta = FColor(0xFFFF00FF);
}  // namespace Colors