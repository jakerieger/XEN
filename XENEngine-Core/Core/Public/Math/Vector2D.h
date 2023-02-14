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