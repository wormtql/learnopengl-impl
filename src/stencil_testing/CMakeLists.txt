add_executable(learnopengl_stencil_testing main.cpp)

target_link_libraries(learnopengl_stencil_testing
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
