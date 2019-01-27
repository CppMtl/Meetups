cmake_minimum_required(VERSION 3.10.0)
project(chr_replace CXX)

include (ExternalProject)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

# Organize unrelated targets to clean IDE hierarchy.
set(DEPENDENCY_FOLDER "Dependencies")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER ${DEPENDENCY_FOLDER})

# Output binary to predictable location.
set(BINARY_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_OUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BINARY_OUT_DIR})

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BINARY_OUT_DIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BINARY_OUT_DIR})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BINARY_OUT_DIR})
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.hpp" "src/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src) # For based paths.

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -msse2)# -Rpass-missed=loop-vectorize)# -Rpass-analysis=loop-vectorize)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -msse2)

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)# /arch:SSE2 always enabled on 64bit builds

endif()

##
# External dependencies.
##
set(LIB_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_INCLUDE_DIR})

ExternalProject_Add(bench_util_ext
	GIT_REPOSITORY https://github.com/p-groarke/bench_util.git
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release #${CMAKE_ARGS}
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
		-DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}
		-DBUILD_EXAMPLES=Off
	UPDATE_COMMAND ""
)

set_target_properties(bench_util_ext PROPERTIES FOLDER ${DEPENDENCY_FOLDER})
add_dependencies(${PROJECT_NAME} bench_util_ext)