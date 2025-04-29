cmake_minimum_required(VERSION 3.19)
project(Lidar-Visualization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt6 경로 찾기
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets core)

qt_standard_project_setup()

add_subdirectory(src)