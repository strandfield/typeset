
enable_testing()

if(NOT DEFINED TYPESET_DOWNLOADED_CATCH2 OR NOT TYPESET_DOWNLOADED_CATCH2)

  message("Downloading Catch2")
  file(DOWNLOAD https://github.com/catchorg/Catch2/releases/download/v2.11.3/catch.hpp "${CMAKE_CURRENT_LIST_DIR}/catch.hpp" SHOW_PROGRESS)
  
  set(TYPESET_DOWNLOADED_CATCH2 TRUE CACHE BOOL "Whether Catch2 was downloaded")

endif()

add_executable(tests catch.hpp main.cpp test-typeset.h test-typeset.cpp test-atom.cpp test-lexer.cpp test-preprocessor.cpp test-format.cpp 
               test-glue-parsing.cpp
               test-parser.cpp)
add_dependencies(tests texnetium)
target_include_directories(tests PUBLIC "../include")
target_link_libraries(tests texnetium)
