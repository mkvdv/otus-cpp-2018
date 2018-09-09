cmake_minimum_required(VERSION 3.8)

if(ENV{TRAVIS_BUILD_NUMBER})
    project(cpp_02 VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(cpp_02 VERSION 0.0.1)
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

add_executable(ip_filter src/ip_filter.cpp src/filters.cpp src/utils.cpp)
add_executable(test_utils src/tests.cpp src/filters.cpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(test_utils PROPERTIES
	CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIR}
)

function(set_warning_flags target)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
                target_compile_options(${target} PRIVATE "-Wall" "-Wextra" "-pedantic-errors")
        elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
                target_compile_options(${target} PRIVATE "/W4")
        endif()
endfunction()

set_warning_flags(ip_filter)
set_warning_flags(test_utils)


target_link_libraries(test_utils
    gtest
    gtest_main
    pthread
)

# install to bin folder our binaries
install(TARGETS ip_filter RUNTIME DESTINATION bin)

# generate .deb package
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT noname@nomail.no)

# # include will generate proper code for .deb generation
include(CPack)

# set up testing
enable_testing()
add_test(tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_utils)
