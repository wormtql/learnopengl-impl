#version 330 core

out vec4 FragColor;
in vec3 Normal;

void main() {
    FragColor = vec4(226.0 / 255, 144.0 / 255, 62.0 / 155, 1.0);
//    FragColor = vec4(Normal, 1.0);
}