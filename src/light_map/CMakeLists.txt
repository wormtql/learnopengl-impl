add_executable(learnopengl_light_map main.cpp)

target_link_libraries(learnopengl_light_map
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
