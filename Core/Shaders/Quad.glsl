R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 FragCoord;

void main() {
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
    FragCoord = aPos;
}
#undef VERTEX

#define FRAGMENT
#version 460 core
in vec3 FragCoord;

uniform vec2 u_Resolution;
uniform float u_Time;
uniform float u_DeltaTime;

out vec4 FragColor;

void main() {
    float red = FragCoord.x * sin(u_Time);
    float green = FragCoord.y * cos(u_Time);
    float blue = FragCoord.z * sin(u_Time);
    FragColor = vec4(red, green, blue, 1.0);
}
#undef FRAGMENT
)"";