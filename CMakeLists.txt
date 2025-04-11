cmake_minimum_required(VERSION 3.18)
project(grubbstest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(grubbstest
    grubbstest/main.cpp
    grubbstest/mainFunctions.cpp
)

target_include_directories(grubbstest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/grubbstest
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)