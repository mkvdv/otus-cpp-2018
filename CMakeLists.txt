cmake_minimum_required(VERSION 3.2)

project(cpp_03 VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

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

add_executable(allocator	./src/main.cpp)
add_executable(unit_tests	./tests/alltests.cpp
        				  	./tests/mylist_tests.cpp
					        ./tests/fixed_size_list_tests.cpp
					        ./tests/allocator_tests.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fuse-ld=gold") # it fails otherwise

set_target_properties(allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-ggdb;-O0;-Wshadow;-Wall;-Wextra;-Werror;-pedantic;-Wfloat-equal;-Winit-self;-Wuninitialized;-Wwrite-strings;-Wcast-align;-Wmissing-include-dirs;-Wswitch-enum;-Wswitch-default;-Winvalid-pch;-Wredundant-decls;-Wformat=2;-Wmissing-format-attribute;-Wformat-nonliteral;-fno-inline-functions;-fmax-errors=3;-Wno-unused-local-typedefs;-fsanitize=address"
)

set_target_properties(unit_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-ggdb;-O0;-Wshadow;-Wall;-Wextra;-Werror;-pedantic;-Wfloat-equal;-Winit-self;-Wuninitialized;-Wwrite-strings;-Wcast-align;-Wmissing-include-dirs;-Wswitch-enum;-Wswitch-default;-Winvalid-pch;-Wredundant-decls;-Wformat=2;-Wmissing-format-attribute;-Wformat-nonliteral;-fno-inline-functions;-fmax-errors=3;-Wno-unused-local-typedefs;-fsanitize=address"
)

target_link_libraries(unit_tests
        gtest
        gtest_main
        pthread
        )

# # set up testing
enable_testing()
add_test(tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unit_tests)


# install to bin folder our binaries
install(TARGETS allocator RUNTIME DESTINATION bin)

# generate .deb package
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT noname@nomail.no)

# include will generate proper code for .deb generation
include(CPack)
