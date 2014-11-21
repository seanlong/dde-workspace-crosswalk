set(MODULE_NAME "dentry")

AUX_SOURCE_DIRECTORY(. Dentry)
set(SRC_LIST ${Dentry})

execute_process(
  COMMAND ${BINDING_GENERATOR} ${CMAKE_CURRENT_SOURCE_DIR}/dentry.cfg ${CMAKE_CURRENT_BINARY_DIR})

add_library(dentry SHARED ${SRC_LIST})
include_directories(${GTK_INCLUDE_DIRS})
target_link_libraries(dentry ${GTK_LIBRARIES} common dcore)
set_target_properties(dentry PROPERTIES LIBRARY_OUTPUT_DIRECTORY /usr/lib/xwalk/lib)

ADD_XWLAK_EXT_LIB(${MODULE_NAME})
