add_executable(learnopengl_point_shadows main.cpp)

target_link_libraries(learnopengl_point_shadows
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
