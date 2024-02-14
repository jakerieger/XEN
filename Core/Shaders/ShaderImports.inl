#pragma once

#include "STL.h"

/**
 * \brief Defines GLSL methods that can be imported in to shaders used by the
 * engine
 */
namespace ShaderImports {
    const inline string Map = R""(
float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
    )"";

    const map<const char*, const string> ImportMap = {{"Map", Map}};
}  // namespace ShaderImports