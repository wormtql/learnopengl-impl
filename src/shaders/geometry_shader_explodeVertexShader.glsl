#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out vec3 Normal;
//out vec3 FragPos;
//out vec2 TexCoords;

out VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
} vs_out;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vs_out.Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

//    Normal = mat3(transpose(inverse(model))) * aNormal;
//    FragPos = vec3(model * vec4(aPos, 1.0));
//    TexCoords = aTexCoords;
}
