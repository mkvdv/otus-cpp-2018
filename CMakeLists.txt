cmake_minimum_required(VERSION 3.8)

if($ENV{TRAVIS_BUILD_NUMBER})
	project(matrix VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
	project(matrix VERSION 0.0.1)
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

add_executable(matrix src/matrix.cpp)
add_executable(unit_tests tests/alltests_runner.cpp tests/matrix_tests.cpp)

macro(set_warning_flags target)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fuse-ld=gold") # otherwise it fails
		set_target_properties(${target} PROPERTIES
			CXX_STANDARD 17
			CXX_STANDARD_REQUIRED ON
			COMPILE_OPTIONS "-Wall;-Wextra;-Werror;-Wpedantic;-fsanitize=address"
		)		
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set_target_properties(${target} PROPERTIES
			CXX_STANDARD 17
			CXX_STANDARD_REQUIRED ON
			COMPILE_OPTIONS "/W4"
		)
	endif()
endmacro()

set_warning_flags(matrix)
set_warning_flags(unit_tests)

target_link_libraries(unit_tests
        gtest
        gtest_main
        pthread )

# set up testing
enable_testing()
add_test(tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unit_tests)

# install to bin folder our binaries
install(TARGETS matrix RUNTIME DESTINATION bin)

# generate .deb package
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT noname@nomail.no)

# include will generate proper code for .deb generation
include(CPack)
