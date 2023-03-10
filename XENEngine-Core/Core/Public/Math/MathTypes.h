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
#include "Vector2D.h"
#include "Vector3D.h"

namespace XEN::Core::FMath {
    
typedef FVector3D FVector;

struct CORE_API FTransform {
    FVector3D Translation;
    FVector3D Rotation;
    FVector3D Scale;
};

struct CORE_API FVertex {
    FVector3D Position;
    FVector3D Normals;
    FVector2D TexCoords;
};
    
}
