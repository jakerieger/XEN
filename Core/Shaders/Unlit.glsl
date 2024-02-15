R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_Projection;

out vec3 FragCoord;
out vec2 TexCoord;

void main() {
    gl_Position = u_MVP * vec4(aPos, 1.0);
    FragCoord = aPos;
    TexCoord = aTexCoord;
}
#undef VERTEX

#define FRAGMENT
#version 460 core
in vec3 FragCoord;
in vec2 TexCoord;

uniform vec2 u_Resolution;
uniform float u_Time;
uniform float u_DeltaTime;

uniform sampler2D u_Texture_0;
uniform sampler2D u_Texture_1;
uniform sampler2D u_Texture_2;
uniform sampler2D u_Texture_3;
uniform sampler2D u_Texture_4;
uniform sampler2D u_Texture_5;
uniform sampler2D u_Texture_6;
uniform sampler2D u_Texture_7;

out vec4 FragColor;

float map(float value, float min1, float max1, float min2, float max2) {
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main() {
    float red = sin(u_Time) * TexCoord.x;
    float green = cos(u_Time) * TexCoord.y;
    red = map(red, -1.0, 1.0, 0.0, 1.0);
    green = map(green, -1.0, 1.0, 0.0, 1.0);
    vec4 color = vec4(red, green, 0.0, 1.0);

    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
#undef FRAGMENT
)"";