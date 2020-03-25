cmake_minimum_required(VERSION 3.5)
project(lcms2)

include(ExternalProject)

externalproject_add(libraw
  SOURCE_DIR ${CMAKE_SOURCE_DIR}
  CONFIGURE_COMMAND CC=${CMAKE_C_COMPILER} AR=${CMAKE_AR}
                    ${CMAKE_SOURCE_DIR}/configure
                      --enable-static
                      --disable-shared
                      --enable-jpeg
                      --with-pic
                      --prefix=${OUTPUT_DIR}
  INSTALL_COMMAND ""
  BUILD_IN_SOURCE 1)

install(CODE "execute_process(COMMAND make install WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})")
