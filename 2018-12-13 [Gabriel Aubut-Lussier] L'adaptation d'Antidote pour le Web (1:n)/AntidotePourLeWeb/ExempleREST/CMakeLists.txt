cmake_minimum_required(VERSION 3.12)

find_package(Boost 1.68 COMPONENTS system)

add_executable(ExempleREST)
set_property(TARGET ExempleREST PROPERTY CXX_STANDARD 17)
target_include_directories(ExempleREST PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${Boost_INCLUDE_DIRS}
)
target_link_libraries(ExempleREST PRIVATE HTTPUtils typestring ${Boost_LIBRARIES})
target_sources(ExempleREST PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include/Address.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/CustomerInfo.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/SingleHandlerServer.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/Address.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/CustomerInfo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/SingleHandlerServer.cpp
)
if (APPLE)
	target_compile_options(ExempleREST PRIVATE "-mmacosx-version-min=10.14")
	set_target_properties(ExempleREST PROPERTIES LINK_FLAGS "-mmacosx-version-min=10.14")
endif ()

