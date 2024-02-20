R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_ObjectColor;

out vec3 FragCoord;
out vec2 TexCoord;
out vec3 Normal;
out vec3 ObjectColor;

void main() {
    mat4 MVP = u_Projection * u_View * u_Model;
    vec4 pos = vec4(aPos, 1.0);
    gl_Position = MVP * pos;
    FragCoord = vec3(u_Model * pos);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    ObjectColor = u_ObjectColor;
}
#undef VERTEX

#define FRAGMENT
#version 460 core
in vec3 FragCoord;
in vec2 TexCoord;
in vec3 Normal;
in vec3 ObjectColor;

uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

out vec4 FragColor;

void main() {

    FragColor = vec4(ObjectColor, 1.0);
}
#undef FRAGMENT
)"";