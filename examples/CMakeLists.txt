
set(LIBTYPESET_BUILD_EXAMPLES FALSE CACHE BOOL "Check if you want to build the examples")

if(LIBTYPESET_BUILD_EXAMPLES)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC TRUE)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

file(GLOB_RECURSE LIBTYPESET_EXAMPLES_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE LIBTYPESET_EXAMPLES_HRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(examples ${LIBTYPESET_EXAMPLES_HRC_FILES} ${LIBTYPESET_EXAMPLES_SRC_FILES} res.qrc)
add_dependencies(examples libtypeset)

target_link_libraries(examples libtypeset)
target_link_libraries(examples Qt5::Core)
target_link_libraries(examples Qt5::Widgets)

set_target_properties(examples PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set_target_properties(examples PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

target_include_directories(examples PUBLIC ".")

endif()


