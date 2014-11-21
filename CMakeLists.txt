cmake_minimum_required(VERSION 2.6)
project(deepin-crosswalk-extensions)

message(${CMAKE_CURRENT_SOURCE_DIR})

set(LIBJSON_C_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/json-c)
include(ExternalProject)
ExternalProject_Add(
  libjson-c
  SOURCE_DIR ${LIBJSON_C_DIR}
  CONFIGURE_COMMAND cd ${LIBJSON_C_DIR} && sh autogen.sh && ${LIBJSON_C_DIR}/configure --libdir=/usr/lib/xwalk/ --includedir=${CMAKE_CURRENT_SOURCE_DIR}/include
  BUILD_COMMAND cd ${LIBJSON_C_DIR} && make
  INSTALL_COMMAND cd ${LIBJSON_C_DIR} && make install
)

set(CMAKE_C_FLAGS "-Wall -Wextra -std=c99")
set(RESOURCE_DIR "${CMAKE_INSTALL_PREFIX}/share/dde/resources")
add_definitions("-DDATA_DIR=\"${CMAKE_INSTALL_PREFIX}/share/dde/data\"")
add_definitions("-DRESOURCE_DIR=\"${RESOURCE_DIR}\"")

find_package(PkgConfig)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

set(INCLUDE_DIRS ${GTK_INCLUDE_DIRS})
set(LIBS ${GTK_LIBRARIES})

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

