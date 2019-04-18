cmake_minimum_required(VERSION 3.12)

add_library(AsyncSystem INTERFACE)
set_property(TARGET AsyncSystem PROPERTY INTERFACE_CXX_STANDARD 17)
target_include_directories(AsyncSystem INTERFACE
		./include
)
target_sources(AsyncSystem INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include/AsyncSystem.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AsyncSystemTestRig.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/VariableCondition.hpp
)

