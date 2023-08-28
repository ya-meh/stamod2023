cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

project(statmod)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME}
        controller/main.cpp
        model/model.h
        gui/gui.h gui/gui.cpp
        internal/probdist.h internal/probdist.cpp internal/vector.h internal/random.h model/model_table.cpp model/model_table.h model/experiment.cpp model/experiment.h internal/pair.h model/model_chen.cpp model/model_chen.h internal/defer.h)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)