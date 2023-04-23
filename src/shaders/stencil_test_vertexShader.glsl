#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;

void main() {
    vec4 position = projection * view * model * vec4(aPos, 1.0);
    vec3 newNormal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = position + vec4(0.002 * newNormal, 0);
//    gl_Position = vec4(position.xyz + 0.002 * newNormal, 1.0);
    Normal = newNormal;
//    gl_Position = position.xyzw + vec4(0.0, 0.1, 0.0, 0.0);
//    gl_Position = vec4(position.xyz + 0.01 * newNormal, 1.0);
//    Normal = newNormal;
}
