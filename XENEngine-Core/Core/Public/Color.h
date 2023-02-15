/**
 * Copyright 2023 Jake Rieger
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "Core.h"
#include <string>
#include <vector>

namespace XEN::Core {

    struct CORE_API FColor {
        // Constructors
        FColor() : R(0), G(0), B(0), A(255) {}
        explicit FColor(uint8 V) : R(V), G(V), B(V), A(255) {}
        FColor(uint8 V, uint8 A) : R(V), G(V), B(V), A(A) {}
        FColor(uint8 R, uint8 G, uint8 B) : R(R), G(G), B(B), A(255) {}
        FColor(uint8 R, uint8 G, uint8 B, uint8 A) : R(R), G(G), B(B), A(A) {}
        explicit FColor(float V);
        FColor(float V, float A);
        FColor(float R, float G, float B);
        FColor(float R, float G, float B, float A);
        explicit FColor(const char* Hex);
        explicit FColor(const std::string& Hex) : FColor(Hex.c_str()) {}

        // Getters / Setters
        void SetRed(uint8 Value);
        void SetGreen(uint8 Value);
        void SetBlue(uint8 Value);
        void SetAlpha(uint8 Value);
        [[nodiscard]] inline uint8 GetRed() const   { return R; };
        [[nodiscard]] inline uint8 GetGreen() const { return G; };
        [[nodiscard]] inline uint8 GetBlue() const  { return B; };
        [[nodiscard]] inline uint8 GetAlpha() const { return A; };

        // Utilities
        char* AsHexCode();
        [[nodiscard]] std::string AsHexCodeStr() const;
        [[nodiscard]] std::vector<float> AsFloatVec() const;
        [[nodiscard]] std::vector<uint8> AsIntVec() const;
        [[nodiscard]] float* AsFloatArray() const;
        [[nodiscard]] uint8* AsIntArray() const;

    private:
        // Store color RGBA values as 8 bit unsigned integers (0-255)
        uint8 R;
        uint8 G;
        uint8 B;
        uint8 A;
    };

}