include_directories(${CMAKE_CURRENT_SOURCE_DIR} "../include")
link_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/out)

set(BINDING_GENERATOR ${PROJECT_SOURCE_DIR}/tools/binding_generator.py)

macro(ADD_XWLAK_EXT_LIB LIB_NAME)
  add_library(${LIB_NAME}-ext SHARED ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}_js_api.c ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}_c_api.c)
  set_target_properties(${LIB_NAME}-ext PROPERTIES LIBRARY_OUTPUT_DIRECTORY /usr/lib/xwalk/extensions)
  target_link_libraries(${LIB_NAME}-ext ${LIB_NAME})
endmacro()

add_subdirectory("common")
add_subdirectory("dcore")
add_subdirectory("dbus")
add_subdirectory("dentry")
add_subdirectory("desktop")
add_subdirectory("dock")
add_subdirectory("launcher")

