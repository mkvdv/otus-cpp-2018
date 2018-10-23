cmake_minimum_required(VERSION 3.10)

project(concepts)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

##########################################################
# set up googletest
execute_process(COMMAND git submodule update --init -- libs/gtest
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# set GTEST_INCLUDE_DIR properly
set(GTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest/googletest/include
        CACHE PATH "path to gtest includes")
# run cmake for gtest too
add_subdirectory(libs/gtest/googletest)
##########################################################

add_executable(tests
        tests/alltests.cpp
        tests/random_access_iterator_tests.cpp
        src/no_default_constructible.cpp)

set_target_properties(tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wall;-Wextra;-Werror;-Wpedantic;-Wno-unused")

target_link_libraries(tests
        gtest
        ${GTEST_MAIN_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        )

# set up testing
enable_testing()
add_test(basic_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests)