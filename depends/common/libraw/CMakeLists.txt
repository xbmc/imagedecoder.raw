cmake_minimum_required(VERSION 2.8)

project(libraw)

include(ExternalProject)

externalproject_add(libraw
  SOURCE_DIR ${CMAKE_SOURCE_DIR}
  CONFIGURE_COMMAND CC=${CMAKE_C_COMPILER} AR=${CMAKE_AR} ${CMAKE_SOURCE_DIR}/configure --disable-examples --prefix=${OUTPUT_DIR}
  INSTALL_COMMAND ""
  BUILD_IN_SOURCE 1)

install(CODE "execute_process(COMMAND make install WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})")
