cmake_minimum_required(VERSION 3.8)

project(timus)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(1196 src/1196.cpp)
add_executable(1496 src/1496.cpp)
add_executable(1545 src/1545.cpp)
add_executable(1837 src/1837.cpp)

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

set_warning_flags(1196)
set_warning_flags(1496)
set_warning_flags(1545)
set_warning_flags(1837)

