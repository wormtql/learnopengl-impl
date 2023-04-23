#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse;

void main() {
    vec3 norm = normalize(Normal);

//    vec4 texColor = texture(texture_diffuse, TexCoords);
//    if (texColor.w < 0.1) {
//        discard;
//    }
    FragColor = texture(texture_diffuse, TexCoords);
//    FragColor = vec4(TexCoords, 1.0, 1.0);
}