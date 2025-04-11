cmake_minimum_required(VERSION 3.15..3.30)
project(GrubbsTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(Boost REQUIRED)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/GrubbsTest)

add_library(GrubbsTest SHARED 
    GrubbsTest/mainFunctions.cpp
)

pybind11_add_module(main MODULE
    GrubbsTest/main.cpp
)

target_include_directories(GrubbsTest 
    PRIVATE ${pybind11_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/GrubbsTest
)

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/GrubbsTest
    ${Python3_INCLUDE_DIRS}
    ${pybind11_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(main PRIVATE
    GrubbsTest
    ${Python3_LIBRARIES}
    ${Boost_LIBRARIES}
)

set_target_properties(main PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/GrubbsTest
)