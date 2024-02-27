R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_LSM;
uniform mat4 u_Model;

void main() {
    gl_Position = u_LSM * u_Model * vec4(aPos, 1.0);
}
#undef VERTEX

#define FRAGMENT
#version 460 core

void main() {
    gl_FragDepth = gl_FragCoord.z;
}
#undef FRAGMENT
)""