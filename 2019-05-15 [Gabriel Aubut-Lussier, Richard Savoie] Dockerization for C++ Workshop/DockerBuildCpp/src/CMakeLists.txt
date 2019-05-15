cmake_minimum_required(VERSION 3.7)

project(hellocpp)
add_executable(hellocpp main.cpp hellostring.cpp)
set_target_properties(hellocpp PROPERTIES CXX_STANDARD 14)

install(TARGETS hellocpp DESTINATION bin)
