add_library(learnopengl_framework STATIC)

#file(COPY ${PROJECT_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_sources(learnopengl_framework
        PUBLIC
        include/framework.h
        include/Shader.h
        include/TextureHelper.h
        include/Mesh.h
        include/Geometry.h
        PRIVATE
        framework.cpp
        Shader.cpp
        TextureHelper.cpp
        Mesh.cpp
        Geometry.cpp
        )

target_include_directories(learnopengl_framework
        PUBLIC
        include)

target_link_libraries(learnopengl_framework
        glfw
        glad
        glm
        stb_image
        assimp
        )
