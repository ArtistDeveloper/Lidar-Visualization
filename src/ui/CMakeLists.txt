add_library(ui STATIC
    MainWindow.cpp MainWindow.h
    buttons/LoadFolderButton.cpp buttons/LoadFolderButton.h
    buttons/MenuButton.cpp buttons/MenuButton.h
    widgets/PointCloudViewer.cpp widgets/PointCloudViewer.h
    widgets/PointCloudPlayerWidget.cpp widgets/PointCloudPlayerWidget.h
    dialogs/ProgressDialog.cpp dialogs/ProgressDialog.h
)

target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/buttons)
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/widgets)
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dialogs)

target_link_libraries(ui 
    PUBLIC Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets mediator
    PRIVATE core graphics
)

qt6_add_resources(ui_resources resources.qrc)
target_sources(ui PRIVATE ${ui_resources})
