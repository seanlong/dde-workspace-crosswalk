set(MODULE_NAME "launcher")

AUX_SOURCE_DIRECTORY(. Launcher)
set(SRC_LIST ${Launcher})

execute_process(
  COMMAND ${BINDING_GENERATOR} ${CMAKE_CURRENT_SOURCE_DIR}/launcher.cfg ${CMAKE_CURRENT_BINARY_DIR})

add_library(launcher SHARED ${SRC_LIST})
include_directories(${GTK_INCLUDE_DIRS})
target_link_libraries(launcher ${GTK_LIBRARIES} common dcore dentry)
set_target_properties(launcher PROPERTIES LIBRARY_OUTPUT_DIRECTORY /usr/lib/xwalk/lib)

ADD_XWLAK_EXT_LIB(${MODULE_NAME})
