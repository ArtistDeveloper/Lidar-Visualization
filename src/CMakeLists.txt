# add_subdirectory는 최상단에 두어야 각 디렉토리에 정의한 타겟을 해당 파일에서 사용 가능
add_subdirectory(ui)
add_subdirectory(core)
add_subdirectory(graphics)
add_subdirectory(io)
add_subdirectory(mediator)
add_subdirectory(controller)

# add_executable에 대한 QT의 래퍼.
# 자동으로 정적 QT Build의 QT 플러그인을 링킹,
# 플랫폼별 라이브러리 이름 사용자 지정 등의 작업을 자동으로 추가하는 로직 제공
qt_add_executable(${PROJECT_NAME}
    main.cpp define.h
)

# resources.qrc 경로는 graphics 하위에 있음
qt_add_resources(app_resources
    "../src/graphics/resources.qrc"
)

# 실행 타겟에 리소스 소스 추가
target_sources(${PROJECT_NAME} PRIVATE ${app_resources})


target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ui
        core
        graphics
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_PREFIX_PATH}/bin/windeployqt.exe"
            "$<TARGET_FILE:${PROJECT_NAME}>"
            --debug
    COMMENT "Running wideployqt to bundle Qt dependencies"
)