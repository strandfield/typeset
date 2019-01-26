
enable_testing()


add_executable(tests test.cpp)
add_dependencies(tests libtypeset)
target_include_directories(tests PUBLIC "${GTEST_DIR}/include")
target_include_directories(tests PRIVATE "${GTEST_DIR}")
target_include_directories(tests PUBLIC "../include")
target_link_libraries(tests libtypeset)
