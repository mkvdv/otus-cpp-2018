cmake_minimum_required(VERSION 3.8)

if (DEFINED ENV{TRAVIS_BUILD_NUMBER} AND DEFINED ENV{VERSION_MAJOR} AND DEFINED ENV{VERSION_MINOR})
	project(bulk VERSION $ENV{VERSION_MAJOR}.$ENV{VERSION_MINOR}.$ENV{TRAVIS_BUILD_NUMBER})
else()
	project(bulk VERSION 1.0.1)
endif()

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

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/command/command.cpp
        src/io/logger.cpp
        src/io/reader.cpp
        src/command/command_pool.cpp
        src/bulk_controller.cpp
        )

add_executable(smoke_tests
        tests/alltests_runner.cpp
        tests/smoke_tests.cpp
        src/command/command.cpp
        src/io/logger.cpp
        src/io/reader.cpp
        src/command/command_pool.cpp
        src/bulk_controller.cpp
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
set_warning_flags(smoke_tests)

target_link_libraries(smoke_tests
        gtest
        ${GTEST_MAIN_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT} )

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

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# include will generate proper code for .deb generation
include(CPack)
