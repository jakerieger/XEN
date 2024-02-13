//
// Created by jr on 2/12/24.
//

#include "Color.h"

FColor::FColor(const uint32_t color) : Red(0.f), Green(0.f), Blue(0.f) {
    Utilities::HexToRGBf(color, Red, Green, Blue, Alpha);
}

FColor::FColor(const uint8_t r,
               const uint8_t g,
               const uint8_t b,
               const uint8_t a) {
    Red   = static_cast<float>(r) / 255.f;
    Green = static_cast<float>(g) / 255.f;
    Blue  = static_cast<float>(b) / 255.f;
    Alpha = static_cast<float>(a) / 255.f;
}

FColor::FColor(const uint8_t v) {
    Red   = static_cast<float>(v) / 255.f;
    Green = static_cast<float>(v) / 255.f;
    Blue  = static_cast<float>(v) / 255.f;
}

FColor::FColor(const float r, const float g, const float b, const float a)
    : Red(r), Green(g), Blue(b), Alpha(a) {}

FColor::FColor(const float v) : Red(v), Green(v), Blue(v) {}