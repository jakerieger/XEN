#define VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
#undef VERTEX

#define FRAGMENT
#version 330 core
void main() {
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
#undef FRAGMENT
