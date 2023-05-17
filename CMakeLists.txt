cmake_minimum_required(VERSION 3.20)
project(learnopengl)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

# glad
add_subdirectory(deps/glad/cmake)
glad_add_library(glad STATIC LOCATION ${CMAKE_CURRENT_LIST_DIR}/generated API gl:core=3.3)

# glfw
add_subdirectory(deps/glfw)

# assimp
add_subdirectory(deps/assimp)

# project
add_subdirectory(src/transformation)
add_subdirectory(src/framework)
add_subdirectory(src/coordinates)
add_subdirectory(src/camera)
add_subdirectory(src/color)
add_subdirectory(src/material)
add_subdirectory(src/light_map)
add_subdirectory(src/light_casters)
add_subdirectory(src/model)
add_subdirectory(src/depth_testing)
add_subdirectory(src/stencil_testing)
add_subdirectory(src/blending)
add_subdirectory(src/face_culling)
add_subdirectory(src/frame_buffers)
add_subdirectory(src/cubemap)
add_subdirectory(src/geometry_shader)
add_subdirectory(src/instancing)
add_subdirectory(src/shadow_mapping)
add_subdirectory(src/point_shadows)
add_subdirectory(src/normal_mapping)
add_subdirectory(src/parallax_mapping)
add_subdirectory(src/hdr)

# glm
add_subdirectory(deps/glm)

# stb_image
add_subdirectory(deps/stb_impl/stb_image)
