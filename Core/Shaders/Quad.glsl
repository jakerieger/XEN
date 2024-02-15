R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_Transform;

out vec3 FragCoord;
out vec2 TexCoord;

void main() {
    gl_Position = u_Transform * vec4(aPos.xy, 0.0, 1.0);
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
    vec4 tex = texture(u_Texture_0, TexCoord);
    vec4 outColor = mix(tex, color, 0.9);
    FragColor = outColor;
    //    FragColor = texture(u_Texture_0, TexCoord);
}
#undef FRAGMENT
)"";