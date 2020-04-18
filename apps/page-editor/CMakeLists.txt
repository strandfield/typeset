
set(TYPESET_BUILD_PAGEEDITOR FALSE CACHE BOOL "Check if you want to build the page-editor app")

if(TYPESET_BUILD_PAGEEDITOR)

  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC TRUE)
  
  find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
  
  file(GLOB_RECURSE TYPESET_PAGEEDITOR_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
  file(GLOB_RECURSE TYPESET_PAGEEDITOR_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
  
  add_executable(page-editor ${TYPESET_PAGEEDITOR_HDR_FILES} ${TYPESET_PAGEEDITOR_SRC_FILES})
  add_dependencies(page-editor texnetium tfm)
  add_dependencies(page-editor app-common)

  target_include_directories(page-editor PUBLIC "..")

  target_link_libraries(page-editor texnetium tfm)
  target_link_libraries(page-editor app-common)
  target_link_libraries(page-editor Qt5::Core Qt5::Gui Qt5::Widgets)

endif()


