add_library(core STATIC
    BinLoader.cpp BinLoader.h
    BinDataLoader.cpp BinDataLoader.h
    PointCloudPlayer.cpp PointCloudPlayer.h
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(core
    PUBLIC Qt6::Widgets
)