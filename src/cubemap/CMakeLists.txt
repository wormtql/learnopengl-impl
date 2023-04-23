add_executable(learnopengl_cubemap main.cpp)

target_link_libraries(learnopengl_cubemap
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
