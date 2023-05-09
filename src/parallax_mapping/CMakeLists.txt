add_executable(learnopengl_parallax_mapping main.cpp)

target_link_libraries(learnopengl_parallax_mapping
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
