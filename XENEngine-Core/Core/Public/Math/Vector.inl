#pragma once

#include <cmath>

#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

// Vector Templates
namespace XEN::Core::FMath {

template<typename T> double CalculateAngleBetween(T const& V1, T const& V2);

template<>
inline double CalculateAngleBetween<FVector2D>(FVector2D const& V1, FVector2D const& V2) {
    const double a_x = V1.X;
    const double a_y = V1.Y;
    const double b_x = V2.X;
    const double b_y = V2.Y;

    return std::acos(
            ((a_x * b_x) + (a_y * b_y)) /
            (
                std::pow((std::pow(a_x, 2) + std::pow(a_y, 2)), 0.5) *
                std::pow((std::pow(b_x, 2) + std::pow(b_y, 2)), 0.5)
            )
    );
}

template<typename T> T& GetUnitVector(T& V);

template<>
inline FVector2D& GetUnitVector(FVector2D& V) {
    return V / V.Magnitude();
}

template<typename T> double Distance(T const& V1, T const& V2);

template<>
inline double Distance<FVector2D>(FVector2D const& V1, FVector2D const& V2) {
    const double x1 = V1.X;
    const double x2 = V2.X;
    const double y1 = V1.Y;
    const double y2 = V2.Y;

    return std::sqrt(
        ((x2 - x1) * (x2 - x1)) +
        ((y2 - y1) * (y2 - y1))
    );
}

template<>
inline double Distance<FVector3D>(FVector3D const& V1, FVector3D const& V2) {
    const double x1 = V1.X;
    const double x2 = V2.X;
    const double y1 = V1.Y;
    const double y2 = V2.Y;
    const double z1 = V1.Z;
    const double z2 = V2.Z;

    return std::sqrt(
        ((x2 - x1) * (x2 - x1)) +
        ((y2 - y1) * (y2 - y1)) +
        ((z2 - z1) * (z2 - z1))
    );
}

template<>
inline double Distance<FVector4D>(FVector4D const& V1, FVector4D const& V2) {
    const double x1 = V1.X;
    const double x2 = V2.X;
    const double y1 = V1.Y;
    const double y2 = V2.Y;
    const double z1 = V1.Z;
    const double z2 = V2.Z;
    const double w1 = V1.W;
    const double w2 = V2.W;

    return std::sqrt(
        ((x2 - x1) * (x2 - x1)) +
        ((y2 - y1) * (y2 - y1)) +
        ((z2 - z1) * (z2 - z1)) +
        ((w2 - w1) * (w2 - w1))
    );
}
    
}