set(MODULE_NAME "dbus")

execute_process(
  COMMAND ${BINDING_GENERATOR} ${CMAKE_CURRENT_SOURCE_DIR}/dbus.cfg ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${GTK_INCLUDE_DIRS})
add_library(${MODULE_NAME}-ext SHARED ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}_js_api.c ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}_c_api.c)
set_target_properties(${MODULE_NAME}-ext PROPERTIES LIBRARY_OUTPUT_DIRECTORY /usr/lib/xwalk/extensions)
