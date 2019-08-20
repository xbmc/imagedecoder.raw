# - Try to find libraw
# Once done this will define
#
# JPEG_FOUND - system has libjpeg
# JPEG_INCLUDE_DIRS - the libjpeg include directory
# JPEG_LIBRARIES - The libjpeg libraries

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_JPEG libjpeg QUIET)
endif()

find_path(JPEG_INCLUDE_DIRS NAMES jpeglib.h
                            PATHS ${PC_JPEG_INCLUDEDIR})
find_library(JPEG_LIBRARIES NAMES jpeg
                            PATHS ${PC_JPEG_LIBDIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JPEG REQUIRED_VARS JPEG_INCLUDE_DIRS JPEG_LIBRARIES)

mark_as_advanced(JPEG_INCLUDE_DIRS JPEG_LIBRARIES)
