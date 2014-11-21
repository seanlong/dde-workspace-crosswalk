set(MODULE_NAME "dcore")

AUX_SOURCE_DIRECTORY(. Dcore)
set(SRC_LIST ${Dcore})

execute_process(
  COMMAND ${BINDING_GENERATOR} ${CMAKE_CURRENT_SOURCE_DIR}/dcore.cfg ${CMAKE_CURRENT_BINARY_DIR})

add_library(dcore SHARED ${SRC_LIST})
include_directories(${GTK_INCLUDE_DIRS})
target_link_libraries(dcore ${LIBS} common /usr/lib/xwalk/libjson-c.so sqlite3)
set_target_properties(dcore PROPERTIES LIBRARY_OUTPUT_DIRECTORY /usr/lib/xwalk/lib)

ADD_XWLAK_EXT_LIB(${MODULE_NAME})
