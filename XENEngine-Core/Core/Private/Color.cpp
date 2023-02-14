#include "Color.h"
#include <format>
#include <cassert>

using namespace XEN::Core;

void FColor::SetRed(uint8 Value)   { this->R = Value; }
void FColor::SetGreen(uint8 Value) { this->G = Value; }
void FColor::SetBlue(uint8 Value)  { this->B = Value; }
void FColor::SetAlpha(uint8 Value) { this->A = Value; }

FColor::FColor(float V, float A) {
    float vClamped = (V > 1.f) ? 1.f : V;
    float aClamped = (A > 1.f) ? 1.f : A;

    uint8 r, g, b, a;
    r = static_cast<uint8>(vClamped) * 255;
    g = static_cast<uint8>(vClamped) * 255;
    b = static_cast<uint8>(vClamped) * 255;
    a = static_cast<uint8>(aClamped) * 255;

    this->R = r;
    this->G = g;
    this->B = b;
    this->A = a;
}

FColor::FColor(float V) {
    float vClamped = (V > 1.f) ? 1.f : V;

    uint8 r, g, b, a;
    r = static_cast<uint8>(vClamped) * 255;
    g = static_cast<uint8>(vClamped) * 255;
    b = static_cast<uint8>(vClamped) * 255;
    a = static_cast<uint8>(vClamped) * 255;

    this->R = r;
    this->G = g;
    this->B = b;
    this->A = a;
}

FColor::FColor(float R, float G, float B) {
    uint8 r, g, b;
    r = static_cast<uint8>(R) * 255;
    g = static_cast<uint8>(G) * 255;
    b = static_cast<uint8>(B) * 255;

    this->R = r;
    this->G = g;
    this->B = b;
    this->A = 255;
}

FColor::FColor(float R, float G, float B, float A) {
    uint8 r, g, b, a;
    r = static_cast<uint8>(R) * 255;
    g = static_cast<uint8>(G) * 255;
    b = static_cast<uint8>(B) * 255;
    a = static_cast<uint8>(A) * 255;

    this->R = r;
    this->G = g;
    this->B = b;
    this->A = a;
}

FColor::FColor(const char *Hex) {
    uint32 r, g, b;
    uint8 result = sscanf(Hex, "#%02x%02x%02x", &r, &g, &b);
    assert(result = 3);

    this->R = static_cast<uint8>(r);
    this->G = static_cast<uint8>(g);
    this->B = static_cast<uint8>(b);
    this->A = 255;
}

char* FColor::AsHexCode() {
    static std::string out = std::format("#{:x}{:x}{:x}{:x}", this->R, this->G, this->B, this->A);
    return (char*)out.c_str();
}

std::string FColor::AsHexCodeStr() const {
    static const std::string out = std::format("#{:x}{:x}{:x}{:x}", this->R, this->G, this->B, this->A);
    return out;
}

std::vector<float> FColor::AsFloatVec() const {
    static const std::vector<float> outVec = { this->R / 255.f, this->G / 255.f, this->B / 255.f, this->A / 255.f };
    return outVec;
}

std::vector<uint8> FColor::AsIntVec() const {
    static const std::vector<uint8> outVec = { this->R, this->G, this->B, this->A };
    return outVec;
}

float* FColor::AsFloatArray() {
    static float outArray[4] = { this->R / 255.f, this->G / 255.f, this->B / 255.f, this->A / 255.f };
    return outArray;
}

uint8* FColor::AsIntArray() {
    static uint8 outArray[4] = { this->R, this->G, this->B, this->A };
    return outArray;
}