cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(QAddApp LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

set(QT5_LIBS Qt5::Core Qt5::Widgets)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(Headers
    mainwindow.h
    )

set(Sources
    mainwindow.cpp
    main.cpp
    )


add_executable(${PROJECT_NAME}
    ${Headers}
    ${Sources}
    )

target_link_libraries(${PROJECT_NAME} PUBLIC ${QT5_LIBS})
