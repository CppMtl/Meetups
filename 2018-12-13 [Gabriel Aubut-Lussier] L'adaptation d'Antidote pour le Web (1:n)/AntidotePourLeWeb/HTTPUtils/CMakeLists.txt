cmake_minimum_required(VERSION 3.5)

find_package(Boost 1.68 COMPONENTS regex system)

add_library(HTTPUtils INTERFACE)
set_property(TARGET HTTPUtils PROPERTY INTERFACE_CXX_STANDARD 17)
target_include_directories(HTTPUtils INTERFACE
	./include
	/usr/local/include
	${Boost_INCLUDE_DIRS}
)
target_sources(HTTPUtils INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include/REST.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/SerialiseurGenerique.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/ValideurGenerique.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/ValideurJSON.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/ValideurJSON.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/ValideurQueryString.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/ValideurQueryString.hpp
)
target_link_libraries(HTTPUtils INTERFACE ${Boost_LIBRARIES})

