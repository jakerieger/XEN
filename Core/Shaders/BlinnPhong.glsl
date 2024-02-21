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
uniform float u_UV_Scale;

uniform sampler2D u_DiffuseMap;

out vec4 FragColor;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(u_LightPosition - FragCoord);

    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * u_LightColor;

    float specularStrength = 1.0;
    vec3 viewDir = normalize(u_ViewPosition - FragCoord);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * u_LightColor;

    vec4 diffMap = texture(u_DiffuseMap, TexCoord * u_UV_Scale);
    vec3 result = (ambient + diffuse + specular) * (ObjectColor * diffMap.xyz);

    FragColor = vec4(result /* * brightness */, 1.0);
}
#undef FRAGMENT
)"";