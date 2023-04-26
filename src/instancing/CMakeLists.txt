add_executable(learnopengl_instancing main.cpp)

target_link_libraries(learnopengl_instancing
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
