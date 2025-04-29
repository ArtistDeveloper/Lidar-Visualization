cmake_minimum_required(VERSION 3.19)
project(Lidar-Visualization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6 경로 찾기
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets core)

qt_standard_project_setup()

# add_executable에 대한 QT의 래퍼.
# 자동으로 정적 QT Build의 QT 플러그인을 링킹,
# 플랫폼별 라이브러리 이름 사용자 지정 등의 작업을 자동으로 추가하는 로직 제공
qt_add_executable(${PROJECT_NAME}
    src/main.cpp
    src/MyOpenGLWidget.cpp
    src/BinLoad.cpp
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