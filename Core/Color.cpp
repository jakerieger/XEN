//
// Created by jr on 2/12/24.
//

#include "Color.h"

FColor::FColor(const u32 color) : Red(0.f), Green(0.f), Blue(0.f) {
    Utilities::HexToRGBAf(color, Red, Green, Blue, Alpha);
}

FColor::FColor(const u8 r,
               const u8 g,
               const u8 b,
               const u8 a) {
    Red   = static_cast<float>(r) / 255.f;
    Green = static_cast<float>(g) / 255.f;
    Blue  = static_cast<float>(b) / 255.f;
    Alpha = static_cast<float>(a) / 255.f;
}

FColor::FColor(const u8 v) {
    Red   = static_cast<float>(v) / 255.f;
    Green = static_cast<float>(v) / 255.f;
    Blue  = static_cast<float>(v) / 255.f;
}

FColor::FColor(const float r, const float g, const float b, const float a)
    : Red(r), Green(g), Blue(b), Alpha(a) {}

FColor::FColor(const float v) : Red(v), Green(v), Blue(v) {}