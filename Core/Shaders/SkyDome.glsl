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
out vec3 Normal;

void main() {
    mat4 MVP = u_Projection * u_View * u_Model;
    vec4 pos = vec4(aPos, 1.0);
    gl_Position = MVP * pos;
    FragCoord = vec3(u_Model * pos);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
}
#undef VERTEX

#define FRAGMENT
#version 460 core
in vec3 FragCoord;
in vec2 TexCoord;
in vec3 Normal;

uniform float u_UV_Scale;
uniform sampler2D u_Texture;

out vec4 FragColor;

void main() {
    vec4 tex = texture(u_Texture, TexCoord * u_UV_Scale);
    FragColor = tex;
}
#undef FRAGMENT
)"";