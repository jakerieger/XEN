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