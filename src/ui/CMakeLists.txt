add_library(ui STATIC
    MyOpenGLWidget.cpp MyOpenGLWidget.h
    MainWindow.cpp MainWindow.h
)

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ui 
    PUBLIC Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets
    PRIVATE core
)