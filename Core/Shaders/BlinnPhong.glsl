R""(
#define VERTEX
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_ObjectColor;
uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;

out vec3 FragCoord;
out vec2 TexCoord;
out vec3 ObjectColor;
out vec3 LightPosition;
out vec3 ViewPosition;
out mat4 View;
out vec3 Normal;

out TangentBitangent {
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} TBN;

void main() {
    mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    FragCoord = vec3(u_Model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    ObjectColor = u_ObjectColor;
    LightPosition = u_LightPosition;
    ViewPosition = u_ViewPosition;
    View = u_View;
    Normal = aNormal;

    mat3 tbnMat = transpose(mat3(T, B, N));
    TBN.TangentLightPos = tbnMat * LightPosition;
    TBN.TangentViewPos = tbnMat * ViewPosition;
    TBN.TangentFragPos = tbnMat * FragCoord;

    mat4 MVP = u_Projection * u_View * u_Model;
    gl_Position = MVP * vec4(aPos, 1.0);
}
#undef VERTEX

#define FRAGMENT
#version 460 core
in vec3 FragCoord;
in vec2 TexCoord;
in vec3 ObjectColor;
in vec3 LightPosition;
in vec3 ViewPosition;
in mat4 View;
in vec3 Normal;

in TangentBitangent {
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} TBN;


uniform vec3 u_LightColor;
uniform float u_LightStrength;
uniform float u_UV_Scale;
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_NormalMap;

out vec4 FragColor;

void main() {
    vec2 texCoord = TexCoord * u_UV_Scale;
    vec3 norm = texture(u_NormalMap, texCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    vec3 lightColor = u_LightColor * u_LightStrength;
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDirection = normalize(TBN.TangentLightPos - TBN.TangentFragPos);

    vec3 color = texture(u_DiffuseMap, texCoord).rgb;
    float diff = max(dot(lightDirection, norm), 0.0);
    vec3 diffuse = diff * color;

    float specularStrength = 1.0;
    vec3 viewDir = normalize(TBN.TangentViewPos - TBN.TangentFragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.2 * spec);

    //    vec3 result = (ambient + diffuse + specular);
    float shadow = 0.0;
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular));

    FragColor = vec4(result, 1.0);
}
#undef FRAGMENT
)"";