cmake_minimum_required(VERSION 3.12)

add_executable(ExempleThreadGuard)
set_property(TARGET ExempleThreadGuard PROPERTY CXX_STANDARD 17)
target_link_libraries(ExempleThreadGuard PRIVATE AsyncSystem ${Boost_LIBRARIES})
target_sources(ExempleThreadGuard PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)
if (APPLE)
	target_compile_options(ExempleThreadGuard PRIVATE "-mmacosx-version-min=10.14")
	set_target_properties(ExempleThreadGuard PROPERTIES LINK_FLAGS "-mmacosx-version-min=10.14")
endif ()

