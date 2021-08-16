#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Upstream::htmlpp" for configuration "Debug"
set_property(TARGET Upstream::htmlpp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Upstream::htmlpp PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libhtmlpp.so.1.0.0"
  IMPORTED_SONAME_DEBUG "libhtmlpp.so.1.0.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS Upstream::htmlpp )
list(APPEND _IMPORT_CHECK_FILES_FOR_Upstream::htmlpp "${_IMPORT_PREFIX}/lib/libhtmlpp.so.1.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
