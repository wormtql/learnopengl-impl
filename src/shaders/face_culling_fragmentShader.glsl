#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

void main() {
    vec3 norm = normalize(Normal);

    float b = 64.0 / 255.0;
    float w = 150.0 / 255.0;
    vec3 black = vec3(b, b, b);
    vec3 white = vec3(w, w, w);

    int count = 15;
    float u = TexCoords.x;
    float v = TexCoords.y;

    float x = floor(u * float(count));
    float y = floor(v * float(count));

    int temp = ((int(x))) + (int(y));
    if (temp % 2 == 0) {
        FragColor = vec4(black, 1.0);
    } else {
        FragColor = vec4(white, 1.0);
    }

    float circleX = (x + 0.5) * (1.0 / float(count));
    float circleY = (y + 0.5) * (1.0 / float(count));

    float r = 0.005;
    float dis = sqrt(pow(u - circleX, 2) + pow(v - circleY, 2));
    if (dis <= r) {
        FragColor = vec4(TexCoords, 1.0, 1.0);
    }
}