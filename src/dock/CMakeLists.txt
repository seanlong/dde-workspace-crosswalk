set(MODULE_NAME "dock")

AUX_SOURCE_DIRECTORY(. Dock)
set(SRC_LIST ${Dock})

execute_process(
  COMMAND ${BINDING_GENERATOR} ${CMAKE_CURRENT_SOURCE_DIR}/dock.cfg ${CMAKE_CURRENT_BINARY_DIR})

add_library(dock SHARED ${SRC_LIST})
include_directories(${GTK_INCLUDE_DIRS})
target_link_libraries(dock ${GTK_LIBRARIES} common dcore dentry)
set_target_properties(dock PROPERTIES LIBRARY_OUTPUT_DIRECTORY /usr/lib/xwalk/lib)

ADD_XWLAK_EXT_LIB(${MODULE_NAME})
