cmake_minimum_required(VERSION 3.8)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(realty VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ()
    project(realty VERSION 0.0.1)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(rclst
        src/rclst.cpp
        src/exceptions/exceptions.cpp
        src/clusterization/clusterization.cpp
        )

add_executable(rclss
        src/rclss.cpp
        src/exceptions/exceptions.cpp
        src/classification/classification.cpp
        )

message("${CMAKE_CXX_COMPILER_ID}")
macro(set_warning_flags target)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message("!!! GCC DETECTED !!!!")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fuse-ld=gold -fsanitize=address -Wall -Wextra -pedantic")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 14
                CXX_STANDARD_REQUIRED ON
                LINK_LIBRARIES pthread
                COMPILE_OPTIONS "-O0;-Wall;-Wextra;-Werror;-Wpedantic;-fsanitize=address;-ggdb"
                )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("!!! CLANG DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 14
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "-O0;-Wall;-Wextra;-Werror;-pedantic;-Wno-unused-variable"
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

set_warning_flags(rclst)
target_link_libraries(rclst
        ${Boost_LIBRARIES}
        dlib
        openblas
        )

set_warning_flags(rclss)
target_link_libraries(rclss
        ${Boost_LIBRARIES}
        dlib
        openblas
        )

# install to bin folder our binaries
install(TARGETS rclst rclss
        RUNTIME
        DESTINATION bin)

# generate .deb package
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT noname@nomail.no)

# include will generate proper code for .deb generation
include(CPack)
