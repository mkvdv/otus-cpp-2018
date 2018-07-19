cmake_minimum_required(VERSION 3.2)

project(cpp_02 VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(ip_filter src/ip_filter.cpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wall -Wextra -Werror -Wpedantic -fsanitize=address
)

# install to bin folder our binaries
install(TARGETS ip_filter RUNTIME DESTINATION bin)

# generate .deb package
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT noname@nomail.no)

# include will generate proper code for .deb generation
include(CPack)
