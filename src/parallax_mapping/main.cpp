#include <iostream>
#include <format>
#include <sstream>
#include <vector>
#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "framework.h"
#include "Shader.h"
#include "TextureHelper.h"
#include "Mesh.h"
#include "Geometry.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

float lastX = 400;
float lastY = 300;
float yaw = 90.0f;
float pitch = 0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch -= yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = -glm::normalize(front);
}

float fov = 45;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;
}

template<typename... Args>
std::string concat(Args&& ... args) {
    std::ostringstream os;
    (os << ... << args);
    return os.str();
}

vector<float> calcTangent(const LearnOpenGLFramework::Quad& quad) {
    using LearnOpenGLFramework::Quad;
    vector<float> result;
    for (int i = 0; i < 2; i++) {
        float* vertex1 = &Quad::vertices[Quad::indices[i * 3] * 8];
        float* vertex2 = &Quad::vertices[Quad::indices[i * 3 + 1] * 8];
        float* vertex3 = &Quad::vertices[Quad::indices[i * 3 + 2] * 8];

        float* vertices[] = { vertex1, vertex2, vertex3 };

        glm::vec3 positions[3];
        glm::vec2 uvs[3];
        glm::vec3 normals[3];
        for (int j = 0; j < 3; j++) {
            positions[j] = glm::vec3(vertices[j][0], vertices[j][1], vertices[j][2]);
            normals[j] = glm::vec3(vertices[j][3], vertices[j][4], vertices[j][5]);
            uvs[j] = glm::vec2(vertices[j][6], vertices[j][7]);
        }
        // 12, 13

        glm::vec3 edge1 = positions[1] - positions[0];
        glm::vec3 edge2 = positions[2] - positions[0];

        glm::vec2 delta_uv1 = uvs[1] - uvs[0];
        glm::vec2 delta_uv2 = uvs[2] - uvs[0];

        glm::mat2 uv_mat = glm::transpose(glm::mat2(delta_uv1, delta_uv2));
        glm::mat2 uv_mat_i = glm::inverse(uv_mat);

        glm::mat3x2 y = glm::transpose(glm::mat2x3(edge1, edge2));

        glm::mat3x2 tb = uv_mat_i * y;
        glm::mat2x3 ttb = glm::transpose(tb);
        glm::vec3 tangent = ttb[0];
        glm::vec3 bitangent = ttb[1];

        for (int a = 0; a < 3; a++) {
            for (int b = 0; b < 3; b++)
                result.push_back(tangent[b]);
            for (int b = 0; b < 3; b++)
                result.push_back(bitangent[b]);
        }
    }
    return result;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    auto sceneShader = LearnOpenGLFramework::Shader(
            "parallax_mapping/vertexShader",
            "parallax_mapping/fragmentShader"
    );

    LearnOpenGLFramework::Quad quad;
    vector<float> tangents = calcTangent(quad);
    glBindVertexArray(quad.VAO);

    unsigned int tangentBuffer;
    glGenBuffers(1, &tangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tangents.size(), tangents.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(3, 3, GL_FLOAT, false, 6 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, false, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);

    // brick texture
    LearnOpenGLFramework::TextureHelper textureHelper;
    auto diffuseMap = textureHelper.createTexture(R"(E:\CLionProjects\learnopengl\images\bricks2.jpg)", GL_TEXTURE0, false, true);
    auto normalMap = textureHelper.createTexture(R"(E:\CLionProjects\learnopengl\images\bricks2_normal.jpg)", GL_TEXTURE1, false, true);
    auto parallaxMap = textureHelper.createTexture(R"(E:\CLionProjects\learnopengl\images\bricks2_disp.jpg)", GL_TEXTURE2, false, true);

    glm::vec3 lightPos(0.5f, 1.0f, 0.3f);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render scene
        glViewport(0, 0, 800, 600);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sceneShader.use();
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
        sceneShader.setUniformMatrix4f("projection", projection);
        glm::mat4 view(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        sceneShader.setUniformMatrix4f("view", view);
        sceneShader.setUniformVec3_2("lightPos", lightPos);
        sceneShader.setUniformVec3_2("viewPos", cameraPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, parallaxMap);
        sceneShader.setUniform1i("diffuseTexture", 0);
        sceneShader.setUniform1i("normalTexture", 1);
        sceneShader.setUniform1i("depthMap", 2);

        glm::mat4 model(1.0f);
//        model = glm::scale(model, glm::vec3(10.0));
//        model = glm::rotate(model, glm::radians(45.f), glm::vec3(1.0, 1.0, 0.0));
        model = glm::rotate(model, glm::radians(10.0f * (float) glfwGetTime()), glm::vec3(1.0, 1.0, 0.0));
        sceneShader.setUniformMatrix4f("model", model);

        quad.draw(sceneShader);

//        glViewport(0, 0, 800, 600);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        quadShader.use();
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, depthMap);
//        quad.draw(quadShader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    LearnOpenGLFramework::terminate();
}
