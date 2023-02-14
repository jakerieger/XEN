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

namespace XEN::Core::FMath {
    struct CORE_API FVector2D {
        // Constructors
        FVector2D() : X(0), Y(0) {}
        explicit FVector2D(double V) : X(V), Y(V) {}
        FVector2D(double X, double Y) : X(X), Y(Y) {}

        // Operator Overloads
        FVector2D& operator+(FVector2D const& other);
        FVector2D& operator+(float Scalar);
        FVector2D& operator-(FVector2D const& other);
        FVector2D& operator-(float Scalar);
        FVector2D& operator/(FVector2D const& other);
        FVector2D& operator/(float Scalar);
        double operator*(FVector2D const& other) const; // Dot product
        FVector2D& operator*(float Scalar);
        bool operator==(FVector2D const& other) const;

        // Math functions
        [[nodiscard]] double DotProduct(FVector2D const& other) const;
        [[nodiscard]] double Magnitude() const;
        [[nodiscard]] double DistanceTo(FVector2D const& other) const;

        // Helpers
        [[nodiscard]] bool IsUnitVector() const;

        // Vector member vars
        double X;
        double Y;
    };
}