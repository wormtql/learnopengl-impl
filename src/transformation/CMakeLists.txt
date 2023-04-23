file(READ shaders/vertexShader.glsl VERTEX_SHADER_SOURCE)
file(READ shaders/fragmentShader.glsl FRAGMENT_SHADER_SOURCE)

configure_file(main.cpp.in main.cpp @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/images/container.jpg ${CMAKE_CURRENT_BINARY_DIR}/container.jpg COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/images/awesomeface.jpg ${CMAKE_CURRENT_BINARY_DIR}/awesomeface.jpg COPYONLY)
add_executable(learnopengl_transformation main.cpp shaders/vertexShader.glsl shaders/fragmentShader.glsl)

#target_include_directories(learnopengl_transformation
#        PUBLIC
#        deps/glfw/include
#        )

target_link_libraries(learnopengl_transformation
        glfw
        glad
        stb_image
        glm
        learnopengl_framework
        )
