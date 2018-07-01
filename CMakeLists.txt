cmake_minimum_required(VERSION 3.2)

project(cpp_01_gtest VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

configure_file(src/version.h.in ../src/version.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

add_executable(helloworld src/main.cpp)
add_library(helloworld_lib src/lib.cpp)
add_executable(test_version src/tests.cpp)


set_target_properties(helloworld PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wall -Wextra -Werror -Wpedantic -fsanitize=address
)

set_target_properties(helloworld_lib PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wall -Wextra -Werror -Wpedantic -fsanitize=address
)

set_target_properties(test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    COMPILE_OPTIONS -Wall -Wextra -Werror -Wpedantic -fsanitize=address
    INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIR}
)

#################################
# link rules
target_link_libraries(helloworld
    helloworld_lib
)

target_link_libraries(test_version
    helloworld_lib
    # ${Boost_LIBRARIES}
    gtest
    gtest_main
    pthread
)

# install to bin folder our binaries
install(TARGETS helloworld RUNTIME DESTINATION bin)

# generate .deb package
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT noname@nomail.no)

# include will generate proper code for .deb generation
include(CPack)

# set up testing
enable_testing()
add_test(test_version_valid ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_version)

