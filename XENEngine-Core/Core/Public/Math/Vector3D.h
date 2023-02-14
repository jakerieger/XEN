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
    struct FVector2D;
    
    struct CORE_API FVector3D {
        FVector3D() : X(0), Y(0), Z(0) {}
        explicit FVector3D(double V) : X(V), Y(V), Z(V) {}
        FVector3D(double X, double Y, double Z) : X(X), Y(Y), Z(Z) {}
        FVector3D(FVector2D const& V);
        FVector3D(FVector2D const& V, float Z);
        FVector3D(FVector2D const& V, double Z);

        // Operator Overloads
        FVector3D& operator+(FVector3D const& other);
        FVector3D& operator+(double Scalar);
        FVector3D& operator-(FVector3D const& other);
        FVector3D& operator-(double Scalar);
        FVector3D& operator/(FVector3D const& other);
        FVector3D& operator/(double Scalar);
        double operator*(FVector3D const& other) const; // Dot product
        FVector3D& operator*(double Scalar);
        bool operator==(FVector3D const& other) const;

        // Math functions
        [[nodiscard]] FVector3D& CrossProduct(FVector3D const& other) const;
        [[nodiscard]] double DotProduct(FVector3D const& other) const;
        [[nodiscard]] double Magnitude() const;

        // Helpers
        [[nodiscard]] bool IsUnitVector() const;
        
        double X;
        double Y;
        double Z;
    };
}
