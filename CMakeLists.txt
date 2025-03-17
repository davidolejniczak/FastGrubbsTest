cmake_minimum_required(VERSION 3.15)
project(IterativeJackKnife VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG QUIET)

if(NOT pybind11_FOUND)
    execute_process(
        COMMAND ${Python3_EXECUTABLE} -c "import pybind11; print(pybind11.get_include())"
        OUTPUT_VARIABLE PYBIND11_INCLUDE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(PYBIND11_INCLUDE_DIR)
        include_directories(${PYBIND11_INCLUDE_DIR})
        message(STATUS "Found pybind11 via Python: ${PYBIND11_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "Could not find pybind11. Please install it with: pip install pybind11")
    endif()
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSL gsl)
include_directories(${GSL_INCLUDE_DIRS})
link_directories(${GSL_LIBRARY_DIRS})

add_library(jackknife_c SHARED 
    IterativeJackKnife/mainFunctions.c
    IterativeJackKnife/mainFunctions.h
)
target_include_directories(jackknife_c PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/IterativeJackKnife
)
target_link_libraries(jackknife_c
    ${GSL_LIBRARIES}
    m
)

pybind11_add_module(main MODULE
    IterativeJackKnife/main.cpp
)

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/IterativeJackKnife
    ${Python3_INCLUDE_DIRS}
)
target_link_libraries(main PRIVATE
    jackknife_c
    ${GSL_LIBRARIES}
    ${Python3_LIBRARIES}
)