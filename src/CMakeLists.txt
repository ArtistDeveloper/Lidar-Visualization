add_subdirectory(ui)

# add_executable에 대한 QT의 래퍼.
# 자동으로 정적 QT Build의 QT 플러그인을 링킹,
# 플랫폼별 라이브러리 이름 사용자 지정 등의 작업을 자동으로 추가하는 로직 제공
qt_add_executable(${PROJECT_NAME}
    main.cpp
)

# QT6 위젯 라이브러리 링크
target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        ui
        Qt6::Widgets 
        Qt6::OpenGL 
        Qt6::OpenGLWidgets
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_PREFIX_PATH}/bin/windeployqt.exe"
            "$<TARGET_FILE:${PROJECT_NAME}>"
            --debug
    COMMENT "Running wideployqt to bundle Qt dependencies"
)