#include <iostream>
#include <format>
#include <sstream>
#include <vector>

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

    auto shader = LearnOpenGLFramework::Shader("light_map_vertexShader", "blending_fragmentShader");

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

//    stbi_set_flip_vertically_on_load(true);
//    LearnOpenGLFramework::Model model("E:\\CLionProjects\\learnopengl\\models\\nanosuit\\nanosuit.obj");
    LearnOpenGLFramework::Model model("E:\\CLionProjects\\learnopengl\\models\\ayaka\\神里绫华.pmx");

    LearnOpenGLFramework::Quad plane;
    LearnOpenGLFramework::Cube cube;

    std::vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3(-1.5f,  0.0f, -0.48f));
    vegetation.push_back(glm::vec3( 1.5f,  0.0f,  0.51f));
    vegetation.push_back(glm::vec3( 0.0f,  0.0f,  0.7f));
    vegetation.push_back(glm::vec3(-0.3f,  0.0f, -2.3f));
    vegetation.push_back(glm::vec3( 0.5f,  0.0f, -0.6f));

    LearnOpenGLFramework::TextureHelper textureHelper{};
    auto textureGrass = textureHelper.createTexture("E:\\CLionProjects\\learnopengl\\images\\grass.png", GL_TEXTURE0, true, true);
    auto textureMarble = textureHelper.createTexture("E:\\CLionProjects\\learnopengl\\images\\marble.jpg", GL_TEXTURE0, false, true);
    auto textureMetal = textureHelper.createTexture("E:\\CLionProjects\\learnopengl\\images\\metal.png", GL_TEXTURE0, false, true);
    auto textureWindow = textureHelper.createTexture("E:\\CLionProjects\\learnopengl\\images\\blending_transparent_window.png", GL_TEXTURE0, true, true);

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

        shader.use();
        shader.setUniformMatrix4f("projection", projection);
        shader.setUniformMatrix4f("view", view);
//        shader.setUniformVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

        // draw plane
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float) glm::radians(90.0), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureMetal);
            shader.setUniform1i("texture_diffuse", 0);
            shader.setUniformMatrix4f("model", model);
        }
        plane.draw(shader);

        // draw boxes
//        {
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, textureMarble);
//            for (int i = 0; i < vegetation.size(); i++) {
//                const auto& position = vegetation[i];
//                glm::mat4 model = glm::mat4(1.0f);
//                model = glm::translate(model, glm::vec3{ 0.0, 0.51, 0.0 });
//                model = glm::translate(model, position);
//                shader.setUniformMatrix4f("model", model);
//                cube.draw(shader);
//            }
//        }

        // draw grass
//        {
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, textureGrass);
//            for (const auto& position: vegetation) {
//                glm::mat4 model = glm::mat4(1.0f);
//                model = glm::translate(model, glm::vec3{ 0.0, 0.51, 0.0 });
//                model = glm::translate(model, position);
//                shader.setUniformMatrix4f("model", model);
//                plane.draw(shader);
//            }
//        }

        // draw model
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
            shader.setUniformMatrix4f("model", model);
        }
        model.draw(shader);

        // draw window
        {
            std::map<float, glm::vec3> sorted;
            for (auto & i : vegetation)
            {
                float distance = glm::length(cameraPos - i);
                sorted[distance] = i;
            }

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureWindow);
            for (auto it = sorted.rbegin(); it != sorted.rend(); it++) {
                auto position = (*it).second;
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3{ 0.0, 0.51, 0.0 });
                model = glm::translate(model, position);
                shader.setUniformMatrix4f("model", model);
                plane.draw(shader);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    LearnOpenGLFramework::terminate();
}
