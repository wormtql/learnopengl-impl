add_executable(learnopengl_light_casters main.cpp)

target_link_libraries(learnopengl_light_casters
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
