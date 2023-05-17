#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Light {
    vec3 Position;
    vec3 Color;
};

uniform sampler2D diffuseTexture;

uniform vec3 viewPos;
uniform Light lights[4];

vec3 colorCalculation(int i) {
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = lights[i].Color;
    vec3 lightPos = lights[i].Position;
    // ambient
    vec3 ambient = 0.0 * color;
    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    float distance = length(fs_in.FragPos - lightPos);
    diffuse *= 1.0 / (distance * distance);

    vec3 lighting = (ambient + diffuse) * color;

    return lighting;
}

void main() {
    vec3 lighting = vec3(0.0);

    for (int i = 0; i < 4; i++) {
        lighting += colorCalculation(i);
    }

//    FragColor = vec4(0.1, 0.2, 0.3, 1.0);
    FragColor = vec4(lighting, 1.0f);
}