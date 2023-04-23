#ifndef LEARNOPENGL_FRAMEWORK_H
#define LEARNOPENGL_FRAMEWORK_H

#include <functional>

namespace LearnOpenGLFramework {
    using RenderFunction = std::function<void()>;

    int render(const RenderFunction&, GLFWwindow*);

    GLFWwindow* init();

    void terminate();
}

#endif //LEARNOPENGL_FRAMEWORK_H
