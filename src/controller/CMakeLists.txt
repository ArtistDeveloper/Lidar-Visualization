add_library(controller
    LoadFolderController.cpp LoadFolderController.h
)

target_include_directories(controller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(controller PUBLIC Qt6::Core io ui)