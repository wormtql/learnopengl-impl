add_executable(learnopengl_depth_testing main.cpp)

target_link_libraries(learnopengl_depth_testing
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
