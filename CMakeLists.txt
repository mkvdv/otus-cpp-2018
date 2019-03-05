cmake_minimum_required(VERSION 3.8)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER} AND DEFINED ENV{VERSION_MAJOR} AND DEFINED ENV{VERSION_MINOR})
    project(range VERSION $ENV{VERSION_MAJOR}.$ENV{VERSION_MINOR}.$ENV{TRAVIS_BUILD_NUMBER})
else ()
    project(range VERSION 0.0.1)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

add_definitions("-std=c++11")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/filter/filters.cpp
        src/utils/utils.cpp
        )

add_executable(simple_tests
        tests/tests.cpp
        src/filter/filters.cpp
        src/utils/utils.cpp
        )

message("${CMAKE_CXX_COMPILER_ID}")
macro(set_warning_flags target)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message("!!! GCC DETECTED !!!!")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -pedantic -ggdb")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 17
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "-O2;-Wall;-Wextra;-Werror;-ggdb"
                )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("!!! CLANG DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 17
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "-O2;-Wall;-Wextra;-Werror;-pedantic"
                )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message("!!! MSVC DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 17
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "/W4"
                )
    else ()
        message(FATAL_ERROR "!!! NO COMPILER DETECTED, WARNING FLAGS NOT SETTED !!!!")
    endif ()
endmacro()

set_warning_flags(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

set_warning_flags(simple_tests)
target_link_libraries(simple_tests
        ${Boost_LIBRARIES}
        ${CONAN_LIBS}
        )

# set up testing
enable_testing()
add_test(tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/simple_tests)

# install to bin folder our binaries
install(TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION bin)

# generate .deb package
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT noname@nomail.no)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# include will generate proper code for .deb generation
include(CPack)
