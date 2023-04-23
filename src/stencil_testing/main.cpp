#include <iostream>
#include <format>
#include <sstream>

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

    auto shader = LearnOpenGLFramework::Shader("light_map_vertexShader", "stencil_test_object_fragmentShader");
    auto pureShader = LearnOpenGLFramework::Shader("stencil_test_vertexShader", "stencil_test_colorFragmentShader");

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

//    stbi_set_flip_vertically_on_load(true);
//    LearnOpenGLFramework::Model model("E:\\CLionProjects\\learnopengl\\models\\nanosuit\\nanosuit.obj");
    LearnOpenGLFramework::Model model("E:\\CLionProjects\\learnopengl\\models\\ayaka\\神里绫华.pmx");

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 view(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glEnable(GL_STENCIL_TEST);
        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xff);
        glStencilMask(0xff);

        // draw objects
        shader.use();
        shader.setUniformMatrix4f("projection", projection);
        shader.setUniformMatrix4f("view", view);
        shader.setUniformVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
            shader.setUniformMatrix4f("model", model);
        }

        // material
        shader.setUniform1f("material.shininess", 64.0f);
        shader.setUniform1i("material.diffuse", 0);
        shader.setUniform1i("material.specular", 1);

        // directional light
        shader.setUniformVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        shader.setUniformVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
        shader.setUniformVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
        shader.setUniformVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);

        // point light
        for (int i = 0; i < 4; i++) {
            auto position = pointLightPositions[i];

            shader.setUniformVec3(std::format("pointLights[{}].position", i), position.x, position.y, position.z);
            shader.setUniformVec3(std::format("pointLights[{}].ambient", i), 0.2f, 0.2f, 0.2f);
            shader.setUniformVec3(std::format("pointLights[{}].diffuse", i), 0.5f, 0.5f, 0.5f);
            shader.setUniformVec3(std::format("pointLights[{}].specular", i), 1.0f, 1.0f, 1.0f);
            shader.setUniform1f(std::format("pointLights[{}].constant", i), 1.0f);
            shader.setUniform1f(std::format("pointLights[{}].linear", i), 0.09f);
            shader.setUniform1f(std::format("pointLights[{}].quadratic", i), 0.032f);
        }

        // spot light
        shader.setUniformVec3("spotLight.position", cameraPos.x, cameraPos.y, cameraPos.z);
        shader.setUniformVec3("spotLight.direction", cameraFront.x, cameraFront.y, cameraFront.z);
        shader.setUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setUniform1f("spotLight.outerCutoff", glm::cos(glm::radians(17.5f)));
        shader.setUniformVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
        shader.setUniformVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
        shader.setUniformVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

        model.draw(shader);

        // draw outline
        glStencilFunc(GL_NOTEQUAL, 1, 0xff);
        glStencilMask(0xff);
//        glDisable(GL_STENCIL_TEST);
        glDisable(GL_DEPTH_TEST);

        pureShader.use();
        pureShader.setUniformMatrix4f("projection", projection);
        pureShader.setUniformMatrix4f("view", view);
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
            pureShader.setUniformMatrix4f("model", model);
        }
        model.draw(pureShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    LearnOpenGLFramework::terminate();
}
