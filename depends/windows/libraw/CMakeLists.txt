cmake_minimum_required(VERSION 3.2)

project(libraw VERSION 0.18.0 LANGUAGES CXX)

if(MSVC)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

add_library(${PROJECT_NAME}
  src/libraw_c_api.cpp
  src/libraw_cxx.cpp
  src/libraw_datastream.cpp
  libraw/libraw_alloc.h
  libraw/libraw_const.h
  libraw/libraw_datastream.h
  libraw/libraw_internal.h
  libraw/libraw_types.h
  libraw/libraw_version.h
  libraw/libraw.h
  internal/dcraw_common.cpp
  internal/dcraw_fileio.cpp
  internal/demosaic_packs.cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libraw>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  INTERFACE
  $<INSTALL_INTERFACE:include/libraw>
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libraw>
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
  _CRT_SECURE_NO_WARNINGS
  _CRT_STDC_NO_DEPRECATE
  _USRDLL
  QT_NO_DEBUG
  _WINDOWS
  UNICODE
  QT_LARGEFILE_SUPPORT
  LIBRAW_BUILDLIB
  LIBRAW_NODLL
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(DIRECTORY
  ${CMAKE_CURRENT_SOURCE_DIR}/libraw
  DESTINATION include
)

install(EXPORT ${PROJECT_NAME}
  FILE
  ${PROJECT_NAME}.cmake
  NAMESPACE
    ${PROJECT_NAME}::
  DESTINATION
    lib/cmake/${PROJECT_NAME}
)

install(FILES
  cmake/${PROJECT_NAME}-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  DESTINATION
    lib/cmake/${PROJECT_NAME}
)

export(TARGETS ${PROJECT_NAME}
  FILE
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  NAMESPACE ${PROJECT_NAME}::
)

if(MSVC AND BUILD_SHARED_LIBS)
  install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION lib)
endif()
