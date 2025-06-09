add_library(mediator STATIC
    PlaybackMediator.cpp PlaybackMediator.h
)

target_include_directories(mediator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mediator 
    PUBLIC
    Qt6::Core
    ui
    core
    io
    controller
)