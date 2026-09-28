cmake_minimum_required(VERSION 3.5)
project(libraw)

include(ExternalProject)

externalproject_add(libraw
  SOURCE_DIR ${CMAKE_SOURCE_DIR}
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env
                    "CC=${CMAKE_C_COMPILER}"
                    "CXX=${CMAKE_CXX_COMPILER}"
                    "AR=${CMAKE_AR}"
                    "CFLAGS=${CMAKE_C_FLAGS}"
                    "CXXFLAGS=${CMAKE_CXX_FLAGS}"
                    "CPPFLAGS=-I${OUTPUT_DIR}/include"
                    "LDFLAGS=-L${OUTPUT_DIR}/lib ${CMAKE_EXE_LINKER_FLAGS}"
                    "PKG_CONFIG_PATH=${OUTPUT_DIR}/lib/pkgconfig"
                    ${CMAKE_SOURCE_DIR}/configure
                      --enable-static
                      --disable-shared
                      --disable-examples
                      --disable-openmp
                      --enable-jpeg
                      --enable-zlib
                      --enable-lcms
                      --with-pic
                      --prefix=${OUTPUT_DIR}
  INSTALL_COMMAND ""
  BUILD_IN_SOURCE 1)

install(CODE "execute_process(COMMAND make install WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})")
