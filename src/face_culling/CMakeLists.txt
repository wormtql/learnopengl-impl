add_executable(learnopengl_face_culling main.cpp)

target_link_libraries(learnopengl_face_culling
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
