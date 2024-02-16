#pragma once

#include "STL.h"

#define RES_FONT "Fonts"
#define RES_AUDIO "Audio"
#define RES_TEXTURE "Textures"
#define RES_3D_MODEL "Models"
#define RES_ROOT ""

namespace Resources {
    void SetCwd(const char* exePath);
    string GetResource(const char* type, const char* name);
}  // namespace Resources