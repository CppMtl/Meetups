cmake_minimum_required(VERSION 3.12)

find_package(Threads)

add_executable(AsyncButton)
set_property(TARGET AsyncButton PROPERTY CXX_STANDARD 17)
target_include_directories(AsyncButton PRIVATE
	external
	include
)
target_link_libraries(AsyncButton PRIVATE AsyncSystem Threads::Threads)
target_sources(AsyncButton PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/external/catch.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/AsyncButton.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/ButtonControlledMissileSystem.h

	${CMAKE_CURRENT_SOURCE_DIR}/src/AsyncButton.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/ButtonControlledMissileSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/SyncTests.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/EventLogComparisonTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/EventConsumptionTest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/EventConsumptionN3785Test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/EventConsumptionP0443Test.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/EventAggregationTest.cpp
)
if (APPLE)
	target_compile_options(AsyncButton PRIVATE "-mmacosx-version-min=10.14")
	set_target_properties(AsyncButton PROPERTIES LINK_FLAGS "-mmacosx-version-min=10.14")
endif ()


