cmake_minimum_required(VERSION 3.5)

project(statmod)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME}
        controller/main.cpp
        model/model.cpp model/model.h
        gui/gui.cpp gui/gui.h
        )

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)