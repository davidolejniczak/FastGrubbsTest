cmake_minimum_required(VERSION 3.15)
project(IterativeJackKnife VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSL gsl QUIET)

if(NOT GSL_FOUND)
    message(FATAL_ERROR
        "GSL library not found! Please install it manually:\n"
        "  Linux: sudo apt-get install libgsl-dev\n"
        "  macOS: brew install gsl\n"
        "  Windows (MSYS2): pacman -S mingw-w64-x86_64-gsl\n"
    )
endif()

include_directories(${GSL_INCLUDE_DIRS})
link_directories(${GSL_LIBRARY_DIRS})

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED QUIET)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/IterativeJackKnife)

add_library(jackknife_c SHARED 
    IterativeJackKnife/mainFunctions.c
)

target_include_directories(jackknife_c PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/IterativeJackKnife
)
target_link_libraries(jackknife_c PRIVATE ${GSL_LIBRARIES})

pybind11_add_module(main MODULE
    IterativeJackKnife/main.cpp
)

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/IterativeJackKnife
    ${Python3_INCLUDE_DIRS}
    ${pybind11_INCLUDE_DIRS}
)

target_link_libraries(main PRIVATE
    jackknife_c
    ${GSL_LIBRARIES}
    ${Python3_LIBRARIES}
)

set_target_properties(main PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/IterativeJackKnife
)