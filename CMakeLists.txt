cmake_minimum_required(VERSION 3.19)
project(Lidar-Visualization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GUI 관련 프로젝트이므로 반드시 설정
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 경로 찾기
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

# 소스 파일
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/MyOpenGLWidget.cpp
    src/MyOpenGLWidget.h
)

# QT6 위젯 라이브러리 링크
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_PREFIX_PATH}/bin/windeployqt.exe"
            "$<TARGET_FILE:${PROJECT_NAME}>"
            --debug
    COMMENT "Running wideployqt to bundle Qt dependencies"
)