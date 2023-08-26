cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

project(statmod)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME}
        controller/main.cpp
        model/model.h model/model.cpp
        gui/gui.h gui/gui.cpp
        model/probdist.h model/probdist.cpp model/vector.h model/random.h model/stat.h model/model_table.cpp model/model_table.h model/experiment.cpp model/experiment.h model/pair.h)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)