# Tests CMakeLists.txt
include(CTest)

# C tests for mainFunctions.c
add_executable(test_main_functions c_tests/test_main_functions.c)
target_link_libraries(test_main_functions 
    jackknife_c
    ${GSL_LIBRARIES}
)
add_test(NAME test_main_functions COMMAND test_main_functions)

# C++ tests (uses mock implementation without pybind11)
add_executable(test_config_classes cpp_tests/test_config_classes_mock.cpp)
target_link_libraries(test_config_classes
    ${GSL_LIBRARIES}
)
add_test(NAME test_config_classes COMMAND test_config_classes)