add_library(core STATIC
    player/PointCloudPlayer.cpp player/PointCloudPlayer.h
    viewer/OrbitCamera.cpp viewer/OrbitCamera.h
    repository/PointCloudRepository.cpp repository/PointCloudRepository.h
)

target_include_directories(core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/player
    ${CMAKE_CURRENT_SOURCE_DIR}/viewer
    ${CMAKE_CURRENT_SOURCE_DIR}/repository
)

target_link_libraries(core
    PUBLIC Qt6::Widgets io
)