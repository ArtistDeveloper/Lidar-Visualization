add_library(ui STATIC
    PointCloudViewer.cpp PointCloudViewer.h
    MainWindow.cpp MainWindow.h
    OpenFolderButton.cpp OpenFolderButton.h
    ProgressDialog.cpp ProgressDialog.h
    PointCloudPlayerWidget.cpp PointCloudPlayerWidget.h
)

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ui 
    PUBLIC Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets
    PRIVATE core graphics
)