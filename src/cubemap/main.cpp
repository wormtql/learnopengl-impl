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

unsigned int loadCubemap(const vector<string>& faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
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



//    unsigned int VAO;
//    unsigned int VBO;
//
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), reinterpret_cast<void*>(0));
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(2, 2, GL_FLOAT, false, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
//    glEnableVertexAttribArray(2);

    LearnOpenGLFramework::Cube cube;

    auto shader = LearnOpenGLFramework::Shader("cubemap_vertexShader", "cubemap_fragmentShader");
    auto shaderBox = LearnOpenGLFramework::Shader("light_map_vertexShader", "cubemap_object_fragmentShader");

    vector<string> faces{
        "E:\\CLionProjects\\learnopengl\\images\\skybox\\right.jpg",
        "E:\\CLionProjects\\learnopengl\\images\\skybox\\left.jpg",
        "E:\\CLionProjects\\learnopengl\\images\\skybox\\top.jpg",
        "E:\\CLionProjects\\learnopengl\\images\\skybox\\bottom.jpg",
        "E:\\CLionProjects\\learnopengl\\images\\skybox\\front.jpg",
        "E:\\CLionProjects\\learnopengl\\images\\skybox\\back.jpg",
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    LearnOpenGLFramework::Model ayaka{"E:\\CLionProjects\\learnopengl\\models\\ayaka\\神里绫华.pmx"};

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 view(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


        // draw object
        {
            shaderBox.use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.1));
            shaderBox.setUniformMatrix4f("projection", projection);
            shaderBox.setUniformMatrix4f("view", view);
            shaderBox.setUniformMatrix4f("model", model);
            shaderBox.setUniformVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

//            glBindVertexArray();
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//            cube.draw(shaderBox);
            ayaka.draw(shaderBox);
        }

        // draw skybox
        {
            shader.use();
            shader.setUniformMatrix4f("projection", projection);
            shader.setUniformMatrix4f("view", glm::mat4(glm::mat3(view)));
            glm::mat4 model = glm::mat4(1.0f);
            glDepthFunc(GL_LEQUAL);
//            glBindVertexArray(VAO);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            shader.setUniformMatrix4f("model", model);
//            glDrawArrays(GL_TRIANGLES, 0, 36);

            cube.draw(shader);

            glDepthFunc(GL_LESS);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    LearnOpenGLFramework::terminate();
}
