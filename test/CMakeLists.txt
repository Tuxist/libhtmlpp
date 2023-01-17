add_executable(csstest csstest.cpp )
target_link_libraries(csstest htmlpp)

add_test(csstest csstest)

add_executable(htmlpagetest htmlpagetest.cpp)
target_link_libraries(htmlpagetest htmlpp)

add_test(htmlpagetest_right htmlpagetest ${CMAKE_SOURCE_DIR}/test/htmlfiles/right.html)
add_test(htmlpagetest_wrong htmlpagetest ${CMAKE_SOURCE_DIR}/test/htmlfiles/wrong.html)

set_tests_properties(htmlpagetest_wrong PROPERTIES WILL_FAIL TRUE)
