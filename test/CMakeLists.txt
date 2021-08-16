add_executable(csstest csstest.cpp)
target_link_libraries(csstest htmlpp-static)

add_executable(htmlpagetest htmlpagetest)
target_link_libraries(htmlpagetest htmlpp-static)
