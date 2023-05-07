#version 330 core
out vec4 color;
in vec2 texCoords;

uniform sampler2D depthMap;

void main()
{
    float depthValue = texture(depthMap, texCoords).r;
    color = vec4(vec3(depthValue), 1.0);
//    color = vec4(0, 0.5, 1.0, 1.0);
}