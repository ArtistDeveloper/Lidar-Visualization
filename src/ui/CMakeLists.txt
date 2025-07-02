add_library(ui STATIC
    MainWindow.cpp MainWindow.h
    buttons/MenuButton.cpp buttons/MenuButton.h
    widgets/PointCloudViewer.cpp widgets/PointCloudViewer.h
    widgets/PointCloudPlayerWidget.cpp widgets/PointCloudPlayerWidget.h
    dialogs/ProgressDialog.cpp dialogs/ProgressDialog.h
)

qt_add_resources(image_sources
    ui_resources.qrc
)

target_sources(ui PUBLIC ${image_sources})

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/buttons)
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/widgets)
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dialogs)

target_link_libraries(ui 
    PUBLIC Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets mediator
    PRIVATE core graphics
)