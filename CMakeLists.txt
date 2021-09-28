project(libhtmlpp)
cmake_minimum_required(VERSION 3.0)
enable_testing ()

find_package(systempp REQUIRED)

set(LIBV "1.0.0")
set(Upstream_VERSION 1.0.0)

if( CMAKE_CXX_COMPILER_ID MATCHES MSVC)
  SET(CMAKE_CXX_FLAGS "-D_SCL_SECURE_NO_WARNINGS  /EHsc")
else()
  set(CMAKE_CXX_FLAGS "-nostdinc++ -nolibc -nostdlib")
endif( CMAKE_CXX_COMPILER_ID MATCHES MSVC)


configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

#add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(test)
