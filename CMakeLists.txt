cmake_minimum_required(VERSION 3.9)
project(typeset)

##################################################################
####### Misc
##################################################################

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # disables warning about DLL export 
  # this is annoying when using C++ STL
  add_compile_options(/wd4251)  
endif()

##################################################################
####### libtypeset library
##################################################################

file(GLOB_RECURSE LIBTYPESET_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE LIBTYPESET_HDR_FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)
add_library(libtypeset ${LIBTYPESET_HDR_FILES} ${LIBTYPESET_SRC_FILES})
target_include_directories(libtypeset PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

foreach(_source IN ITEMS ${LIBTYPESET_HDR_FILES} ${LIBTYPESET_SRC_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

target_compile_definitions(libtypeset PUBLIC -DLIBTYPESET_BUILD_LIB)

##################################################################
####### Examples & Tests
##################################################################

add_subdirectory(examples)
add_subdirectory(tests)
