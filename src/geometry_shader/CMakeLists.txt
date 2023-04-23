add_executable(learnopengl_geometry_shader main.cpp)

target_link_libraries(learnopengl_geometry_shader
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )

add_executable(learnopengl_geometry_shader_explode explode.cpp)

target_link_libraries(learnopengl_geometry_shader_explode
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )