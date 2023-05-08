add_executable(learnopengl_normal_mapping main.cpp)

target_link_libraries(learnopengl_normal_mapping
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
