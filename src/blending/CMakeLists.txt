add_executable(learnopengl_blending main.cpp)

target_link_libraries(learnopengl_blending
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
