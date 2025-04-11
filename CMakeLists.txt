cmake_minimum_required(VERSION 3.18)
project(grubbstest_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_grubbstest_impl
    grubbstest/main.cpp
    grubbstest/mainFunctions.cpp
)

target_include_directories(_grubbstest_impl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/grubbstest
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

set_target_properties(_grubbstest_impl PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/grubbstest)
install(TARGETS _grubbstest_impl DESTINATION grubbstest)