add_executable(learnopengl_hdr main.cpp)

target_link_libraries(learnopengl_hdr
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
