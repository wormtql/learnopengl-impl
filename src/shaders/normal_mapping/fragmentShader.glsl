#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Tangent;
    vec3 BiTangent;
} fs_in;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

void main() {
//    FragColor = vec4(0.5, 0.1, 0.6, 1.0);

    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = texture(normalTexture, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2 - 1.0);

//    FragColor = vec4(normal, 1.0);

    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.15 * lightColor;
    // Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    vec3 lighting = (ambient + (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);

//    if (diffuse == 0) {
//        FragColor = vec4(0.1, 0.5, 0.3, 1.0);
//    } else {
//        FragColor = vec4(diffuse * color, 1.0);
//    }
//    FragColor = vec4(diffuse * color, 1.0);

//    FragColor = vec4(fs_in.BiTangent * 0.5 + 0.5, 1.0);
//    FragColor = vec4(dot(fs_in.Normal, fs_in.BiTangent) * 0.5 + 0.5, 0.0, 0.0, 0.0);
//    FragColor = vec4(diff, 0.0, 0.0, 0.0);
}