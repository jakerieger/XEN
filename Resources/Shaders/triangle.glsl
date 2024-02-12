#define VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
#undef VERTEX

#define FRAGMENT
#version 330 core

uniform vec4 ourColor;

void main() {
    gl_FragColor = ourColor;
}
#undef FRAGMENT
