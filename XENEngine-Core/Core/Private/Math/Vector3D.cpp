#include "Math/Vector3D.h"

#include "Math/Vector.inl"
#include "Math/Vector2D.h"

namespace XEN::Core::FMath {

FVector3D::FVector3D(FVector2D const& V) {
    this->X = V.X;
    this->Y = V.Y;
    this->Z = 0;
}

FVector3D::FVector3D(FVector2D const& V, double Z) {
    this->X = V.X;
    this->Y = V.Y;
    this->Z = Z;
}

FVector3D::FVector3D(FVector2D const& V, float Z) {
    this->X = V.X;
    this->Y = V.Y;
    this->Z = static_cast<double>(Z);  
}

FVector3D& FVector3D::operator+(FVector3D const& other) {
    this->X += other.X;
    this->Y += other.Y;
    this->Z += other.Z;
    return *this;
}
    
FVector3D& FVector3D::operator+(double Scalar) {
    this->X += Scalar;
    this->Y += Scalar;
    this->Z += Scalar;
    return *this;
}
    
FVector3D& FVector3D::operator-(FVector3D const& other) {
    this->X -= other.X;
    this->Y -= other.Y;
    this->Z -= other.Z;
    return *this;
}
    
FVector3D& FVector3D::operator-(double Scalar) {
    this->X -= Scalar;
    this->Y -= Scalar;
    this->Z -= Scalar;
    return *this;
}
    
FVector3D& FVector3D::operator/(FVector3D const& other) {
    this->X /= other.X;
    this->Y /= other.Y;
    this->Z /= other.Z;
    return *this;
}
    
FVector3D& FVector3D::operator/(double Scalar) {
    this->X /= Scalar;
    this->Y /= Scalar;
    this->Z /= Scalar;
    return *this;
}
    
double FVector3D::operator*(FVector3D const& other) const {
    return DotProduct(other);
}
    
FVector3D& FVector3D::operator*(double Scalar) {
    this->X *= Scalar;
    this->Y *= Scalar;
    this->Z *= Scalar;
    return *this;
}
    
bool FVector3D::operator==(FVector3D const& other) const {
    return (this->X == other.X) && (this->Y == other.Y) && (this->Z == other.Z);
}
    
FVector3D& FVector3D::CrossProduct(FVector3D const& other) const {
    static FVector3D outVec;
    outVec.X = (this->Y * other.Z) - (this->Z * other.Y);
    outVec.Y = (this->Z * other.X) - (this->X * other.Z);
    outVec.Z = (this->X * other.Y) - (this->Y * other.X);

    return outVec;
}
    
double FVector3D::DotProduct(FVector3D const& other) const {
    return (this->X * other.X) +
           (this->Y * other.Y) +
           (this->Z * other.Z);
}
    
double FVector3D::Magnitude() const {
    double a_sqr = this->X * this->X;
    double b_sqr = this->Y * this->Y;
    double c_sqr = this->Z * this->Z;

    return std::sqrt(a_sqr + b_sqr + c_sqr);
}
    
bool FVector3D::IsUnitVector() const {
    return this->Magnitude() == 1;
}
    
}