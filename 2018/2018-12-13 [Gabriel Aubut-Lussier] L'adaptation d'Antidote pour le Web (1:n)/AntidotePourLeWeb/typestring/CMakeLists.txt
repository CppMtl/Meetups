cmake_minimum_required(VERSION 3.5)

add_library(typestring INTERFACE)
set_property(TARGET typestring PROPERTY INTERFACE_CXX_STANDARD 17)
target_include_directories(typestring INTERFACE
	./include
)
target_sources(typestring INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include/typestring.h
)

