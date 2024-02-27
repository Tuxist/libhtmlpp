project(libhtmlpp)
cmake_minimum_required(VERSION 3.0)
enable_testing ()

set(LIBV "1.0.0")
set(Upstream_VERSION 1.0.0)

SET(CMAKE_CXX_FLAGS "-fPIC -Wall -fstack-check -fstack-protector -fsanitize=address")

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

#add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(test)
