add_executable(learnopengl_frame_buffers main.cpp)

target_link_libraries(learnopengl_frame_buffers
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
