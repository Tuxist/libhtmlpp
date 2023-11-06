set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include(GenerateExportHeader)

set(libhtmlSrcs
    css.cpp
    html.cpp
    request.cpp
    exception.cpp
    request/ajax.cpp
    request/httpform.cpp
    request/websocket.cpp
)

add_library(htmlpp        SHARED ${libhtmlSrcs} )
target_compile_definitions(htmlpp PUBLIC -DSHARED)

add_library(htmlpp-static STATIC ${libhtmlSrcs} )

generate_export_header(htmlpp)


target_link_libraries(htmlpp c)
target_link_libraries(htmlpp-static c)


SET(CMAKE_INSTALL_LIBDIR lib CACHE PATH "Output directory for libraries")

set_property(TARGET htmlpp PROPERTY VERSION ${Upstream_VERSION})
set_property(TARGET htmlpp PROPERTY SOVERSION ${LIBV})
set_property(TARGET htmlpp PROPERTY INTERFACE_htmlpp_MAJOR_VERSION ${LIBV})
set_property(TARGET htmlpp APPEND PROPERTY COMPATIBLE_INTERFACE_STRING htmlpp_MAJOR_VERSION)

install(TARGETS htmlpp EXPORT htmlppTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include
)

install(TARGETS htmlpp-static
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(FILES
    css.h
    html.h
    request.h
    utils.h
    exception.h
    encode.h
    "${CMAKE_BINARY_DIR}/config.h"
    "${CMAKE_CURRENT_BINARY_DIR}/htmlpp_export.h"
    DESTINATION include/htmlpp
    COMPONENT Devel
)
install(FILES
    request/ajax.h
    request/httpform.h
    request/websocket.h
    DESTINATION include/htmlpp/request
    COMPONENT Devel
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/htmlpp/htmlppConfigVersion.cmake"
  VERSION ${Upstream_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT htmlppTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/htmlpp/htmlppTargets.cmake"
  NAMESPACE Upstream::
)

configure_file(cmake/htmlppConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/htmlpp/htmlppConfig.cmake"
  @ONLY
)

set(ConfigPackageLocation lib/cmake/htmlpp)
install(EXPORT htmlppTargets
  FILE
    htmlppTargets.cmake
  NAMESPACE
    Upstream::
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/htmlpp/htmlppConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/htmlpp/htmlppConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)


