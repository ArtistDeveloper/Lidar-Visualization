add_library(io STATIC
    PointTypes.h
    KittiBinFileReader.cpp KittiBinFileReader.h
    KittiBinDirectoryLoader.cpp KittiBinDirectoryLoader.h
)

target_include_directories(io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(io 
    PUBLIC Qt6::Core
)