AUX_SOURCE_DIRECTORY(. Common)
set(SRC_LIST ${Common})

add_library(common SHARED ${SRC_LIST})
set_target_properties(common PROPERTIES LIBRARY_OUTPUT_DIRECTORY /usr/lib/xwalk/lib)
include_directories(${GTK_INCLUDE_DIRS})
target_link_libraries(common ${GTK_LIBRARIES})
