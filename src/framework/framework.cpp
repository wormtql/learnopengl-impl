#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "framework.h"

namespace LearnOpenGLFramework {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void processInput(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    GLFWwindow* init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
        if (window == nullptr) {
            std::cout << "Failed to create window" << std::endl;
            glfwTerminate();
            return nullptr;
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGL(glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return nullptr;
        }

        glViewport(0, 0, 800, 600);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        return window;
    }

    void terminate() {
        glfwTerminate();
    }

    int render(const RenderFunction &render_function, GLFWwindow* window) {
        while (!glfwWindowShouldClose(window)) {
            processInput(window);

            glClearColor(0.2, 0.3, 0.3, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            render_function();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        return 0;
    }
}

