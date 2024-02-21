R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aTexCoords;
}

#undef VERTEX

#define FRAGMENT
#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_Texture;

vec3 aces(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main() {
    vec4 tex = texture(u_Texture, TexCoords);
    FragColor = vec4(aces(tex.xyz), 1.0);
}
#undef FRAGMENT
)"";