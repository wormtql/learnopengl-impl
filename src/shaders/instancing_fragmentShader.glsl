#version 330 core

struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;
uniform vec3 viewPos;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

const vec3 lightDir = vec3(1, 1, 1);
const vec3 lightAmbient = vec3(0.2f, 0.2f, 0.2f);
const vec3 lightDiffuse = vec3(0.5f);

void main() {
    vec3 ambient = lightAmbient * vec3(texture(material.texture_diffuse1, TexCoords));

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightDiffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));

//    vec3 viewDir = normalize(viewPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 specular = texture(material.specular, TexCoords).rgb * spec * light.specular;

    vec3 result = (ambient + diffuse);
    FragColor = vec4(result, 1.0);

//    FragColor = texture(material.texture_diffuse1, TexCoords);
}