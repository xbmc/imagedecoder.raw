cmake_minimum_required(VERSION 3.5)

project(libraw)

include(ExternalProject)

externalproject_add(libraw
  SOURCE_DIR ${CMAKE_SOURCE_DIR}
  UPDATE_COMMAND autoreconf -vif
  CONFIGURE_COMMAND CC=${CMAKE_C_COMPILER} AR=${CMAKE_AR}
                    ${CMAKE_SOURCE_DIR}/configure
                      --disable-examples
                      --disable-shared
                      --disable-jasper
                      --disable-openmp
                      --enable-jpeg
                      --with-pic
                      --prefix=${OUTPUT_DIR}
                      "CXXFLAGS=${CMAKE_CXX_FLAGS} -std=c++11"
  INSTALL_COMMAND ""
  BUILD_IN_SOURCE 1)

install(CODE "execute_process(COMMAND make install WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})")
