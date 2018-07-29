cmake_minimum_required(VERSION 3.2)

project(cpp_02 VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(allocator src/tests.cpp)

set_target_properties(allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wall;-Wextra;-Werror;-pedantic"
)

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

# # set up testing
enable_testing()
add_test(tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/allocator)