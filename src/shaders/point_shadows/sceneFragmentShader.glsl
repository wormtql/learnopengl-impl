#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

float shadowCalculation(vec3 fragPos, vec3 normal, vec3 lightDir) {
    float shadow = 0.0;
    float bias = 0.05;
    float samples = 4.0;
    float offset = 0.1;

    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

    for (float x = -offset; x < offset; x += offset / (samples * 0.5)) {
        for (float y = -offset; y < offset; y += offset / (samples * 0.5)) {
            for (float z = -offset; z < offset; z += offset / (samples * 0.5)) {
                float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r;
                closestDepth *= far_plane;
                if (currentDepth - bias > closestDepth) {
                    shadow += 1.0;
                }
            }
        }
    }

//
//    float closestDepth = texture(depthMap, fragToLight).r;
//    closestDepth *= far_plane;
//
//
//
//    float bias = 0.05;
//    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

//    FragColor = vec4(vec3(closestDepth / far_plane), 1.0);
//    FragColor = vec4(normalize(fragToLight), 1.0);

    return shadow / (samples * samples * samples);
}

void main() {
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.15 * color;
    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    // Shadow
    float shadow = shadowCalculation(fs_in.FragPos, normal, lightDir);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

//    FragColor = vec4(1.0 - texture(depthMap, -lightDir).rgb, 1.0);

    FragColor = vec4(lighting, 1.0f);
//        FragColor = vec4(vec3(1.0 - shadow), 1.0);
//    FragColor = vec4((diffuse + ambient + specular) * color, 1.0);
}