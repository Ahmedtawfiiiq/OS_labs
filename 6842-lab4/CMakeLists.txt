cmake_minimum_required(VERSION 3.5)

project(6842-lab4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(6842-lab4 main.cc algorithms.cc tokenizer.cc process.cc)

set_target_properties(6842-lab4 PROPERTIES OUTPUT_NAME "lab3")

install(TARGETS 6842-lab4
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})