cmake_minimum_required(VERSION 3.8)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(bulkmt VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else ()
    project(bulkmt VERSION 0.0.1)
endif ()

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

find_package(Threads)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/command/command.cpp
        src/io/reader.cpp
        src/command/command_pool.cpp
        src/bulk_controller.cpp
        src/io/logger_job.cpp
        src/utils/counter.cpp
        src/io/file_logger_job.cpp
        )

add_executable(smoke_tests
        tests/alltests_runner.cpp
        tests/smoke_tests.cpp
        src/command/command.cpp
        src/io/reader.cpp
        src/command/command_pool.cpp
        src/bulk_controller.cpp
        src/io/logger_job.cpp
        src/utils/counter.cpp
        src/io/file_logger_job.cpp
        )

macro(set_warning_flags target)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fuse-ld=gold -Wall -Wextra -Werror -Wpedantic -fsanitize=address -ggdb")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 17
                CXX_STANDARD_REQUIRED ON
                LINK_LIBRARIES pthread
                COMPILE_OPTIONS "-Wall;-Wextra;-Werror;-Wpedantic;-fsanitize=address;-ggdb;-O0"
                )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD 17
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "/W4"
                )
    endif ()
endmacro()

set_warning_flags(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
        )

set_warning_flags(smoke_tests)
target_link_libraries(smoke_tests
        gtest
        ${GTEST_MAIN_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        )

# set up testing
enable_testing()
add_test(tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/smoke_tests)

# install to bin folder our binaries
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# generate .deb package
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT noname@nomail.no)

# include will generate proper code for .deb generation
include(CPack)