
set(LIBTYPESET_BUILD_FORMATEDITOR FALSE CACHE BOOL "Check if you want to build the format-editor")

if(LIBTYPESET_BUILD_FORMATEDITOR)

  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC TRUE)
  
  find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
  
  file(GLOB_RECURSE LIBTYPESET_FORMATEDITOR_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
  file(GLOB_RECURSE LIBTYPESET_FORMATEDITOR_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
  
  add_executable(format-editor ${LIBTYPESET_FORMATEDITOR_HDR_FILES} ${LIBTYPESET_FORMATEDITOR_SRC_FILES})
  add_dependencies(format-editor libtypeset)
  
  target_link_libraries(format-editor libtypeset)
  target_link_libraries(format-editor Qt5::Core Qt5::Gui Qt5::Widgets)

endif()


