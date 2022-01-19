project(libhtmlpp)
cmake_minimum_required(VERSION 3.0)
enable_testing ()

include(CheckIncludeFileCXX)

find_package(systempp REQUIRED)

set(LIBV "1.0.0")
set(Upstream_VERSION 1.0.0)

CHECK_INCLUDE_FILE_CXX("${systempp_INCLUDE_DIR}/config.h" GLIBSUPPORT)

if(GLIBSUPPORT MATCHES OFF)
    SET(CMAKE_CXX_FLAGS "-nolibc -nostdlib") #goal -nolibc
endif()


configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

#add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(test)
