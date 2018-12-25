cmake_minimum_required(VERSION 3.8)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER} AND DEFINED ENV{VERSION_MAJOR} AND DEFINED ENV{VERSION_MINOR})
    project(join_server VERSION $ENV{VERSION_MAJOR}.$ENV{VERSION_MINOR}.$ENV{TRAVIS_BUILD_NUMBER})
else ()
    project(join_server VERSION 0.0.1)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
find_package(Boost COMPONENTS system program_options log unit_test_framework REQUIRED)
find_package(Threads)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/server/server.cpp
        src/model/simple_table.cpp
        src/model/two_tables_storage.cpp
        src/jobs/request_job.cpp
        src/utils/utils.cpp
        tests/table_tests.cpp
        src/server/session.cpp
        )

add_executable(test_table
        tests/table_tests.cpp
        src/model/simple_table.cpp
        )

message("${CMAKE_CXX_COMPILER_ID}")
macro(set_warning_flags target)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message("!!! GCC DETECTED !!!!")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fuse-ld=gold -fsanitize=address -Wall -Wextra -pedantic")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 17
                CXX_STANDARD_REQUIRED ON
                LINK_LIBRARIES pthread
                COMPILE_OPTIONS "-O0;-Wall;-Wextra;-Werror;-Wpedantic;-fsanitize=address;-ggdb"
                )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("!!! CLANG DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 17
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "-O0;-Wall;-Wextra;-Werror;-pedantic"
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
target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

set_target_properties(test_table PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )
target_link_libraries(test_table ${Boost_LIBRARIES})


# set up testing
enable_testing()
add_test(unit_table_testing ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_table)

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
