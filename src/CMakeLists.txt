set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include(GenerateExportHeader)

set(libhtmlSrcs
    css.cpp
    html.cpp
    request.cpp
    exception.cpp
)

add_library(htmlpp        SHARED ${libhtmlSrcs} )
target_compile_definitions(htmlpp PUBLIC -DSHARED)

add_library(htmlpp-static STATIC ${libhtmlSrcs} )

generate_export_header(htmlpp)

target_link_libraries(htmlpp)
target_link_libraries(htmlpp-static)


SET(CMAKE_INSTALL_LIBDIR lib CACHE PATH "Output directory for libraries")

set_property(TARGET htmlpp PROPERTY VERSION ${Upstream_VERSION})
set_property(TARGET htmlpp PROPERTY SOVERSION ${LIBV})
set_property(TARGET htmlpp PROPERTY INTERFACE_htmlpp_MAJOR_VERSION ${LIBV})
set_property(TARGET htmlpp APPEND PROPERTY COMPATIBLE_INTERFACE_STRING htmlpp_MAJOR_VERSION)
 
if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
   install(TARGETS htmlpp EXPORT htmlppTargets RUNTIME DESTINATION lib)
else()
   install(TARGETS htmlpp EXPORT htmlppTargets LIBRARY DESTINATION lib)
endif()

install(TARGETS htmlpp-static DESTINATION lib  EXPORT htmlppTargets)

install(FILES
    css.h
    html.h
    request.h
    utils.h
    exception.h
    "${CMAKE_BINARY_DIR}/config.h"
    "${CMAKE_CURRENT_BINARY_DIR}/htmlpp_export.h"
    DESTINATION include/htmlpp
    COMPONENT Devel
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/libhtmlpp/htmlppConfigVersion.cmake"
  VERSION ${Upstream_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT htmlppTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/libhtmlpp/htmlppTargets.cmake"
  NAMESPACE htmlpp::
)

configure_file(cmake/htmlppConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/libhtmlpp/htmlppConfig.cmake"
  @ONLY
)

set(ConfigPackageLocation lib/cmake/libhtmlpp)

install(EXPORT htmlppTargets
  FILE
    htmlppTargets.cmake
  NAMESPACE
    htmlpp::
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libhtmlpp/htmlppConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libhtmlpp/htmlppConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    set(CPACK_GENERATOR WIX)
    set(CPACK_WIX_VERSION 3)
    set(CPACK_PACKAGE_NAME "libhtmlpp")
    set(CPACK_PACKAGE_VENDOR "tuxist.de")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libhtmlpp - A C++ Html and Css Library")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PACKAGE_VERSION_MAJOR "1")
    set(CPACK_PACKAGE_VERSION_MINOR "0")
    set(CPACK_PACKAGE_VERSION_PATCH "0")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "libhtmlpp")
    INCLUDE(CPack)
endif()
