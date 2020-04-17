
set(LIBTYPESET_BUILD_EQUATIONEDITOR FALSE CACHE BOOL "Check if you want to build the equation-editor")

if(LIBTYPESET_BUILD_EQUATIONEDITOR)

  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC TRUE)
  
  find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
  
  file(GLOB_RECURSE LIBTYPESET_EQUATIONEDITOR_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
  file(GLOB_RECURSE LIBTYPESET_EQUATIONEDITOR_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
  
  add_executable(equation-editor ${LIBTYPESET_EQUATIONEDITOR_HDR_FILES} ${LIBTYPESET_EQUATIONEDITOR_SRC_FILES})
  add_dependencies(equation-editor texnetium tfm)
  
  target_link_libraries(equation-editor texnetium tfm)
  target_link_libraries(equation-editor Qt5::Core Qt5::Gui Qt5::Widgets)

endif()


