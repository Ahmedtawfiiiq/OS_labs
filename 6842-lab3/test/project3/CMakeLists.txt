cmake_minimum_required(VERSION 3.5)

project(project3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(project3 main.cc utilities.cc)

set_target_properties(project3 PROPERTIES OUTPUT_NAME "lab3")

install(TARGETS project3
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})