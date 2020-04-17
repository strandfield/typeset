
set(TYPESET_BUILD_APPCOMMON FALSE CACHE BOOL "Check if you want to build the app-common library")

if(TYPESET_BUILD_APPCOMMON)

  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC TRUE)
  
  find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
  
  file(GLOB_RECURSE LTYPESET_APPCOMMON_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
  file(GLOB_RECURSE LTYPESET_APPCOMMON_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
  
  add_library(app-common STATIC ${LTYPESET_APPCOMMON_HDR_FILES} ${LTYPESET_APPCOMMON_SRC_FILES})
  add_dependencies(app-common texnetium tfm)
  
  target_link_libraries(app-common texnetium tfm)
  target_link_libraries(app-common Qt5::Core Qt5::Gui Qt5::Widgets)

endif()


