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
uniform sampler2D depthMap;

uniform float height_scale = 0.1;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
//    float height = texture(depthMap, texCoords).r;
////    vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
//    vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
//    return texCoords - p;

    const float numLayers = 10;
    float layerDepth = 1.0 / numLayers;

    float currentLayerDepth = 0.0;
    vec2 p = viewDir.xy * height_scale;
    vec2 deltaTexCoords = p / numLayers;

    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, texCoords).r;

    while (currentLayerDepth < currentDepthMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main() {
//    FragColor = vec4(0.5, 0.1, 0.6, 1.0);
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = ParallaxMapping(fs_in.TexCoords, viewDir);

    vec3 color = texture(diffuseTexture, texCoords).rgb;
    vec3 normal = texture(normalTexture, texCoords).rgb;
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