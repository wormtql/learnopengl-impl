add_executable(learnopengl_color main.cpp)

add_dependencies(learnopengl_color learnopengl_framework)

target_link_libraries(learnopengl_color
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
