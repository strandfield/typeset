
set(LIBTYPESET_BUILD_LINEBREAKSVIEWER FALSE CACHE BOOL "Check if you want to build the linebreaks-viewer")

if(LIBTYPESET_BUILD_LINEBREAKSVIEWER)

  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC TRUE)
  
  find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
  
  file(GLOB_RECURSE LIBTYPESET_LINEBREAKSVIEWER_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
  file(GLOB_RECURSE LIBTYPESET_LINEBREAKSVIEWER_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
  
  add_executable(linebreaks-viewer ${LIBTYPESET_LINEBREAKSVIEWER_HDR_FILES} ${LIBTYPESET_LINEBREAKSVIEWER_SRC_FILES})
  add_dependencies(linebreaks-viewer texnetium tfm)
  add_dependencies(linebreaks-viewer app-common)

  target_include_directories(linebreaks-viewer PUBLIC "..")

  target_link_libraries(linebreaks-viewer texnetium tfm)
  target_link_libraries(linebreaks-viewer app-common)
  target_link_libraries(linebreaks-viewer Qt5::Core Qt5::Gui Qt5::Widgets)

endif()


