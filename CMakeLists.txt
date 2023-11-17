cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

project(statmod)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets Charts REQUIRED)

add_executable(${PROJECT_NAME}
        cmd/main.cpp
        internal/probdist.h internal/probdist.cpp internal/vector.h internal/random.h internal/pair.h internal/defer.h
        model/model.h model/model_table.cpp model/model_table.h model/experiment.cpp model/experiment.h model/model_chen.cpp model/model_chen.h
        view/p_val_graph_widget.h view/graph_config.h controller/model_runner.h view/main_window.h controller/p_val_model_runner.h view/modeling_graph_widget.h controller/modeling_model_runner.h view/comparing_histogram.h)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Charts)