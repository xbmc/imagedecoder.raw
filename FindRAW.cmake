# - Try to find libraw
# Once done this will define
#
# RAW_FOUND - system has libraw
# RAW_INCLUDE_DIRS - the libraw include directory
# RAW_LIBRARIES - The libraw libraries

find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_check_modules (RAW libraw_r)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RAW DEFAULT_MSG RAW_INCLUDE_DIRS RAW_LIBRARIES)

mark_as_advanced(RAW_INCLUDE_DIRS RAW_LIBRARIES)
