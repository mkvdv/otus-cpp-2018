cmake_minimum_required(VERSION 3.2)

if (DEFINED TRAVIS_BUILD_NUMBER AND DEFINED VERSION_MAJOR AND DEFINED VERSION_MINOR)
    project(hello_world VERSION $ENV{VERSION_MAJOR}.$ENV{VERSION_MINOR}.$ENV{TRAVIS_BUILD_NUMBER})
else ()
    project(hello_world VERSION 1.0.1)
endif ()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

configure_file(src/version.h.in ../src/version.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(${PROJECT_NAME}
        src/main.cpp
        )

add_library(helloworld_lib
        src/lib.cpp)

add_executable(test_version
        src/test_main.cpp)

message("${CMAKE_CXX_COMPILER_ID}")
macro(set_warning_flags target)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message("!!! GCC DETECTED !!!!")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fuse-ld=gold -fsanitize=address -Wall -Wextra -pedantic")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 14
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "-O0;-Wall;-Wextra;-Werror;-Wpedantic;-fsanitize=address;-ggdb"
                )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("!!! CLANG DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 14
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "-O0;-Wall;-Wextra;-Werror;-pedantic"
                )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message("!!! MSVC DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 14
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "/W4"
                )
    else ()
        message(FATAL_ERROR "!!! NO COMPILER DETECTED, WARNING FLAGS NOT SETTED !!!!")
    endif ()
endmacro()

set_warning_flags(${PROJECT_NAME})
set_warning_flags(test_version)

target_link_libraries(test_version
        ${Boost_LIBRARIES}
        )

# link rules
target_link_libraries(${PROJECT_NAME}
        helloworld_lib
        )

target_link_libraries(test_version
        helloworld_lib
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )

# install to bin folder our binaries
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# generate .deb package
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT noname@nomail.no)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# include will generate proper code for .deb generation
include(CPack)

# set up testing
enable_testing()
add_test(test_version_valid ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_version)
