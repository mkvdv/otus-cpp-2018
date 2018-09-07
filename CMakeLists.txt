cmake_minimum_required(VERSION 3.2)

project(cpp_04 VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(print_ip	./src/main.cpp ./src/utils.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fuse-ld=gold") # otherwise it fails

set_target_properties(print_ip PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-O0;-Wshadow;-Wall;-Wextra;-Werror;-pedantic;-Wfloat-equal;-Winit-self;-Wuninitialized;-Wwrite-strings;-Wcast-align;-Wmissing-include-dirs;-Wswitch-enum;-Wswitch-default;-Winvalid-pch;-Wredundant-decls;-Wformat=2;-Wmissing-format-attribute;-Wformat-nonliteral;-fno-inline-functions;-fmax-errors=3;-Wno-unused-local-typedefs;-fsanitize=address"
)

# set up testing
enable_testing()
add_test(run_main ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/print_ip)

# install to bin folder our binaries
install(TARGETS print_ip RUNTIME DESTINATION bin)

# generate .deb package
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT noname@nomail.no)

# include will generate proper code for .deb generation
include(CPack)
