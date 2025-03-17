cmake_minimum_required(VERSION 3.15)
project(IterativeJackKnife VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG QUIET)

if(NOT pybind11_FOUND)
    # Try alternate method of finding pybind11
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

# Find GSL
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSL gsl)
if(NOT GSL_FOUND)
    message(FATAL_ERROR "GSL not found. Please install GSL library.")
endif()

include_directories(${GSL_INCLUDE_DIRS})
link_directories(${GSL_LIBRARY_DIRS})

# Add the C library
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

# Python module
if(pybind11_FOUND)
    # Use pybind11 targets if found via find_package
    pybind11_add_module(main MODULE
        IterativeJackKnife/main.cpp
    )
else()
    # Fallback to manual module creation
    add_library(main MODULE
        IterativeJackKnife/main.cpp
    )
    set_target_properties(main PROPERTIES
        PREFIX ""
        SUFFIX ${Python3_EXTENSION_SUFFIX}
    )
endif()

# Common properties for the main module
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/IterativeJackKnife
    ${Python3_INCLUDE_DIRS}
)
target_link_libraries(main PRIVATE
    jackknife_c
    ${GSL_LIBRARIES}
    ${Python3_LIBRARIES}
)

# For direct development, output to IterativeJackKnife directory
if(NOT SKBUILD)
    set_target_properties(main PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/IterativeJackKnife
    )
endif()

# Install targets - Handle different installation paths for scikit-build vs regular CMake
if(DEFINED SKBUILD)
    # Installing with scikit-build-core
    install(TARGETS jackknife_c main
        RUNTIME DESTINATION IterativeJackKnife COMPONENT python
        LIBRARY DESTINATION IterativeJackKnife COMPONENT python
        ARCHIVE DESTINATION IterativeJackKnife COMPONENT python
    )
    
    # Install Python files
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IterativeJackKnife/__init__.py
        DESTINATION IterativeJackKnife
        COMPONENT python
    )
else()
    # Regular CMake install
    install(TARGETS jackknife_c main
        RUNTIME DESTINATION lib/IterativeJackKnife COMPONENT python
        LIBRARY DESTINATION lib/IterativeJackKnife COMPONENT python
        ARCHIVE DESTINATION lib/IterativeJackKnife COMPONENT python
    )
    
    # Install Python files
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/IterativeJackKnife/__init__.py
        DESTINATION lib/IterativeJackKnife
        COMPONENT python
    )
endif()
