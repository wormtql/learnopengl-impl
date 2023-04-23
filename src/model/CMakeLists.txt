add_executable(learnopengl_model main.cpp)

target_link_libraries(learnopengl_model
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
