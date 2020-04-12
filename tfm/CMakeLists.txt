
add_library(tfm STATIC 
            "${CMAKE_CURRENT_LIST_DIR}/include/tex/tfm.h"
            "${CMAKE_CURRENT_LIST_DIR}/src/tfm.cpp")

target_include_directories(tfm PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
add_dependencies(tfm libtypeset)
target_include_directories(tfm PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../include")
target_link_libraries(tfm libtypeset)
