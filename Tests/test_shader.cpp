//
// Created by conta on 2/23/2024.
//

#include "GraphicsContext.h"

#include <catch2/catch_test_macros.hpp>
#include "Shader.h"
#include <EASTL/string.h>

constexpr auto TEST_SHADER = R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aPos, 1.0);
}
#undef VERTEX

#define FRAGMENT
#version 460 core

void main()
{
}
#undef FRAGMENT

#define GEOMETRY
#version 460 core


layout(triangles, invocations = 5) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};
/*
uniform mat4 lightSpaceMatrices[16];
*/

void main()
{
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
		gl_Layer = gl_InvocationID;
		EmitVertex();
	}
	EndPrimitive();
}
#undef GEOMETRY
)"";

static void InitOpenGL() {
    Graphics::Initialize({100, 100}, "");
}

static void ShutdownOpenGL() {
    Graphics::Shutdown();
}

TEST_CASE("Geometry Shaders", "[SHADERS]") {
    InitOpenGL();
    const AShader* shader = new AShader(eastl::string(TEST_SHADER));

    delete shader;
    ShutdownOpenGL();
}