#include "Math/Vector2D.h"
#include "Math/MathDefines.h"
#include <cmath>

#include "Math/Vector.inl"

using namespace XEN::Core::FMath;

FVector2D& FVector2D::operator+(FVector2D const& other) {
    this->X += other.X;
    this->Y += other.Y;

    return *this;
}

FVector2D& FVector2D::operator+(float Scalar) {
    this->X += static_cast<double>(Scalar);
    this->Y += static_cast<double>(Scalar);

    return *this;
}

FVector2D& FVector2D::operator-(FVector2D const& other) {
    this->X -= other.X;
    this->Y -= other.Y;

    return *this;
}

FVector2D& FVector2D::operator-(float Scalar) {
    this->X -= static_cast<double>(Scalar);
    this->Y -= static_cast<double>(Scalar);

    return *this;
}

FVector2D& FVector2D::operator/(FVector2D const& other) {
    this->X /= other.X;
    this->Y /= other.Y;

    return *this;
}

FVector2D& FVector2D::operator/(float Scalar) {
    this->X /= static_cast<double>(Scalar);
    this->Y /= static_cast<double>(Scalar);

    return *this;
}

double FVector2D::operator*(FVector2D const& other) const {
    return DotProduct(other);
}

FVector2D& FVector2D::operator*(float Scalar) {
    this->X *= static_cast<double>(Scalar);
    this->Y *= static_cast<double>(Scalar);

    return *this;
}

bool FVector2D::operator==(FVector2D const& other) const {
    return (this->X == other.X) && (this->Y == other.Y);
}

double FVector2D::DotProduct(FVector2D const& other) const {
    return (this->X * other.X) +
           (this->Y * other.Y);
}

double FVector2D::Magnitude() const {
    double a_sqr = this->X * this->X;
    double b_sqr = this->Y * this->Y;

    return std::sqrt(a_sqr + b_sqr);
}

double FVector2D::DistanceTo(FVector2D const& other) const {
    const double x1 = this->X;
    const double x2 = other.X;
    const double y1 = this->Y;
    const double y2 = other.Y;

    return std::sqrt(
        ((x2 - x1) * (x2 - x1)) +
        ((y2 - y1) * (y2 - y1))
    );
}

bool FVector2D::IsUnitVector() const {
    return this->Magnitude() == 1;
}