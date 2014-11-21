set(MODULE_NAME "desktop")

AUX_SOURCE_DIRECTORY(. Desktop)
set(SRC_LIST ${Desktop})

execute_process(
  COMMAND ${BINDING_GENERATOR} ${CMAKE_CURRENT_SOURCE_DIR}/desktop.cfg ${CMAKE_CURRENT_BINARY_DIR})

add_library(desktop SHARED ${SRC_LIST})
include_directories(${GTK_INCLUDE_DIRS})
target_link_libraries(desktop ${GTK_LIBRARIES} common dcore dentry)
set_target_properties(desktop PROPERTIES LIBRARY_OUTPUT_DIRECTORY /usr/lib/xwalk/lib)

ADD_XWLAK_EXT_LIB(${MODULE_NAME})
