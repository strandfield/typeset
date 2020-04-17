
set(LIBTYPESET_BUILD_EXAMPLES FALSE CACHE BOOL "Check if you want to build the examples")

if(LIBTYPESET_BUILD_EXAMPLES)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC TRUE)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

file(GLOB_RECURSE LIBTYPESET_EXAMPLES_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE LIBTYPESET_EXAMPLES_HRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(examples ${LIBTYPESET_EXAMPLES_HRC_FILES} ${LIBTYPESET_EXAMPLES_SRC_FILES} res.qrc)
add_dependencies(examples texnetium tfm)

target_link_libraries(examples texnetium tfm)
target_link_libraries(examples Qt5::Core)
target_link_libraries(examples Qt5::Gui)
target_link_libraries(examples Qt5::Widgets)

endif()


