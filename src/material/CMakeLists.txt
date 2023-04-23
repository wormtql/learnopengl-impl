add_executable(learnopengl_material main.cpp)

file(COPY ${PROJECT_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(learnopengl_material
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
