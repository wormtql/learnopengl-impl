#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 viewPos;
uniform vec3 lightPos;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Tangent;
    vec3 BiTangent;
} vs_out;

void main() {
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(vec3(normalMatrix * tangent));
    vec3 B = normalize(vec3(normalMatrix * bitangent));
    vec3 N = normalize(vec3(normalMatrix * normal));
    mat3 TBN = mat3(T, B, N);

    vs_out.Tangent = T;
    vs_out.BiTangent = B;

    mat3 TTBN = transpose(TBN);
    vs_out.FragPos = vec3(model * vec4(position, 1.0));
    vs_out.Normal = normalize(normalMatrix * normal);
    vs_out.TexCoords = texCoords;

//    vs_out.TangentLightPos = lightPos;
//    vs_out.TangentViewPos = viewPos;
//    vs_out.TangentFragPos = vec3(model * vec4(position, 1.0));

    vs_out.TangentLightPos = TTBN * lightPos;
    vs_out.TangentViewPos = TTBN * viewPos;
    vs_out.TangentFragPos = TTBN * vec3(model * vec4(position, 1.0));

    gl_Position = projection * view * model * vec4(position, 1.0);
}