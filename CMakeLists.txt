cmake_minimum_required(VERSION 3.1.0)

project(FileExtractor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS,5.1")

find_package(Qt5Widgets CONFIG REQUIRED)

set(Qrc_Files
    source/image_source.qrc
    source/qss_source.qrc
    )

set(PROJECT_SRCS
    main.cpp
    mainwindow.cpp
    FileChecker.cpp
    log/log4z.cpp
    )

set(PROJECT_UIS
    mainwindow.ui
    )
#加上这句，不然代码中找不到资源相对路径
qt5_add_resources(QRC_SOURCE_FILES ${Qrc_Files})
file( GLOB LogoFiles source/logo.rc )

add_executable(${PROJECT_NAME}
    WIN32
    ${PROJECT_SRCS}
    ${PROJECT_UIS}
    ${QRC_SOURCE_FILES}
    ${LogoFiles}
    )

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    )

