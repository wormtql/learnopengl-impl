add_executable(learnopengl_shadow_mapping main.cpp)

target_link_libraries(learnopengl_shadow_mapping
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
