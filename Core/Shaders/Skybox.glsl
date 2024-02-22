R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 TexCoord;

void main() {
    TexCoord = aPos;
    vec4 pos = u_Projection * u_View * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
#undef VERTEX

#define FRAGMENT
#version 460 core
out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube u_Skybox;

void main() {
    FragColor = texture(u_Skybox, TexCoord);
}
#undef FRAGMENT
)"";