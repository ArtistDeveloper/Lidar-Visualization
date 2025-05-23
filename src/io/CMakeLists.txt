add_library(io STATIC
    BinLoader.cpp BinLoader.h
    BinDataLoader.cpp BinDataLoader.h
)

target_include_directories(io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(io 
    PUBLIC Qt6::Core
)