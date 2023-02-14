#pragma once

#include "Core.h"
#include "Vector3D.h"

namespace XEN::Core::FMath {
    
typedef FVector3D FVector;

struct CORE_API FTransform {
    FVector3D Translation;
    FVector3D Rotation;
    FVector3D Scale;

    FTransform() : Translation(FVector3D()), Rotation(FVector3D()), Scale(FVector3D(1.f)) {}
};
    
}
