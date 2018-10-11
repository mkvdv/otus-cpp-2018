cmake_minimum_required(VERSION 3.8)

if($ENV{TRAVIS_BUILD_NUMBER})
	project(cpp_05 VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
	project(cpp_05 VERSION 0.0.1)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(editor 	src/main.cpp
				        src/utils/geomfactory.cpp
				        src/model/model.cpp
				        src/controller/controller.cpp
				        src/view/view.cpp
				        src/view/geometry_elements.cpp
				        src/view/view_elements.cpp
				        src/tester.cpp
				        src/utils/logger.cpp
    )

macro(set_warning_flags target)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fuse-ld=gold") # otherwise it fails
		set_target_properties(editor PROPERTIES
			CXX_STANDARD 17
			CXX_STANDARD_REQUIRED ON
			COMPILE_OPTIONS "-Wall;-Wextra;-Werror;-Wpedantic;-fsanitize=address"
		)		
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set_target_properties(editor PROPERTIES
			CXX_STANDARD 17
			CXX_STANDARD_REQUIRED ON
			COMPILE_OPTIONS "/W4"
		)
	endif()
endmacro()

set_warning_flags(editor)

# set up testing
enable_testing()
add_test(run_main ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/editor)

# install to bin folder our binaries
install(TARGETS editor RUNTIME DESTINATION bin)

# generate .deb package
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT noname@nomail.no)

# include will generate proper code for .deb generation
include(CPack)