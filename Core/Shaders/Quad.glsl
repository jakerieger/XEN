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
uniform float u_NearZ;
uniform float u_FarZ;

float LinearDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * u_NearZ * u_FarZ) / (u_FarZ + u_NearZ - z * (u_FarZ - u_NearZ));
}

void main() {
    float depthValue = texture(u_Texture, TexCoords).r;
    FragColor = vec4(vec3(depthValue), 1.0);
    //    FragColor = vec4(vec3(LinearDepth(depthValue) / u_FarZ), 1.0);
}
#undef FRAGMENT
)"";