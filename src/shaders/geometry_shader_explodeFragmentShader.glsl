#version 330 coreaw

out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse0;
};

uniform Material material;

void main() {
    //    vec4 texColor = texture(texture_diffuse, TexCoords);
    //    if (texColor.w < 0.1) {
    //        discard;
    //    }
    FragColor = texture(material.texture_diffuse0, TexCoords);
    //    FragColor = vec4(TexCoords, 1.0, 1.0);
}