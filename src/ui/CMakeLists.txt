add_library(ui STATIC
    MyOpenGLWidget.cpp
)

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ui PUBLIC Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets)