#configure_file(${PROJECT_SOURCE_DIR}/deps/stb/stb_image.h ./stb_image.h COPYONLY)

add_library(stb_image "")

target_sources(stb_image
        PRIVATE
        stb_image.cpp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/deps/stb/stb_image.h
        )

target_include_directories(stb_image
        PUBLIC
        ${PROJECT_SOURCE_DIR}/deps/stb
        )
