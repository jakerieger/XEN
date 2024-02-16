R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 FragCoord;
out vec2 TexCoord;

void main() {
    mat4 MVP = u_Projection * u_View * u_Model;
    gl_Position = MVP * vec4(aPos, 1.0);
    FragCoord = aPos;
    TexCoord = aTexCoord;
}
#undef VERTEX

#define FRAGMENT
#version 460 core
in vec3 FragCoord;
in vec2 TexCoord;

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
#undef FRAGMENT
)"";