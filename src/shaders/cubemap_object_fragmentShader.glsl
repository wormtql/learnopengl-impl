#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main() {
    vec3 norm = normalize(Normal);

    float ratio = 1.00 / 1.52;

//    vec3 I = normalize(viewPos - FragPos);
    vec3 I = normalize(FragPos - viewPos);
//    vec3 R = reflect(I, norm);
    vec3 R = refract(I, norm, ratio);
    FragColor = vec4(texture(skybox, R).xyz, 1.0);
//    FragColor = vec4(norm, 1.0);
}