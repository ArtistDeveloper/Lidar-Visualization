add_library(graphics STATIC
    ShaderProgram.cpp ShaderProgram.h
)

target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(graphics
    PUBLIC Qt6::OpenGL
)