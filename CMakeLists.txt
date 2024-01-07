cmake_minimum_required(VERSION 3.14)
project(Kinematics)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES main.cpp
        Arm/Arm.cpp
        Arm/Arm.h
        Joint/Joint.cpp
        Joint/Joint.h
        Link/Link.cpp
        Link/Link.h
        Game/Game.cpp
        Game/Game.h
        ../libs/imgui-master/imgui.cpp
        ../libs/imgui-master/imgui_widgets.cpp
        ../libs/imgui-master/imgui_draw.cpp
        ../libs/imgui-master/imgui_tables.cpp
        ../libs/imgui-master/imgui-SFML.cpp
        Motion/Motion.h
        Motion/Motion.cpp)
add_executable(Kinematics ${SOURCE_FILES})
include_directories(/usr/local/include ../libs ../libs/imgui-master)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(Kinematics sfml-system sfml-window sfml-graphics sfml-audio sfml-network ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})