include("${CMAKE_CURRENT_LIST_DIR}/htmlppTargets.cmake")

get_filename_component(htmlpp_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
if(EXISTS "${htmlpp_CMAKE_DIR}/CMakeCache.txt")
  # In build tree
  include("${htmlpp_CMAKE_DIR}/htmlppBuildTreeSettings.cmake")
else()
  set(htmlpp_INCLUDE_DIR "/usr/local/include/htmlpp")
endif()

set(htmlpp_LIBRARIES htmlpp)
set(htmlpp_INCLUDE_DIRs ${htmlpp_INCLUDE_DIR})

