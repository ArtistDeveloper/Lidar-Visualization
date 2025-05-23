add_library(ui STATIC
    MainWindow.cpp MainWindow.h
    OpenFolderButton.cpp OpenFolderButton.h
    widgets/PointCloudViewer.cpp widgets/PointCloudViewer.h
    widgets/PointCloudPlayerWidget.cpp widgets/PointCloudPlayerWidget.h
    dialogs/ProgressDialog.cpp dialogs/ProgressDialog.h
)

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/widgets)
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dialogs)

target_link_libraries(ui 
    PUBLIC Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets
    PRIVATE core graphics
)