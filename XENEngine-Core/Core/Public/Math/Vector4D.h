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
    struct FVector3D;
    
    struct CORE_API FVector4D {
        FVector4D() : X(0), Y(0), Z(0), W(0) {}
        explicit FVector4D(double V) : X(V), Y(V), Z(V), W(V) {}
        FVector4D(double X, double Y, double Z, double W) : X(X), Y(Y), Z(Z), W(W) {}
        FVector4D(FVector3D const& V);
        FVector4D(FVector3D const&V, double W);

        double X;
        double Y;
        double Z;
        double W;
    };
}