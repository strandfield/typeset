cmake_minimum_required(VERSION 3.9)
project(typeset)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##################################################################
####### Misc
##################################################################

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # disables warning about DLL export 
  # this is annoying when using C++ STL
  add_compile_options(/wd4251)  
endif()

##################################################################
###### coverage build
##################################################################

if(ENABLE_COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
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
####### TFM
##################################################################

add_subdirectory(tfm)

##################################################################
####### Tools
##################################################################

add_subdirectory(apps)

##################################################################
####### Examples & Tests
##################################################################

add_subdirectory(examples)
add_subdirectory(tests)
