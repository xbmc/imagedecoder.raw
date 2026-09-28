cmake_minimum_required(VERSION 3.5)
project(libraw VERSION 0.22.2 LANGUAGES CXX)

find_package(Threads REQUIRED)
find_package(JPEG REQUIRED)
find_package(lcms2 CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

# Match the library source list in upstream Makefile.am.
add_library(libraw STATIC
  src/libraw_c_api.cpp
  src/libraw_datastream.cpp
  src/decoders/canon_600.cpp
  src/decoders/crx.cpp
  src/decoders/pana8.cpp
  src/decoders/decoders_dcraw.cpp
  src/decoders/sonycc.cpp
  src/decompressors/losslessjpeg.cpp
  src/decoders/decoders_libraw_dcrdefs.cpp
  src/decoders/olympus14.cpp
  src/decoders/decoders_libraw.cpp
  src/decoders/dng.cpp
  src/decoders/fp_dng.cpp
  src/decoders/fuji_compressed.cpp
  src/decoders/generic.cpp
  src/decoders/kodak_decoders.cpp
  src/decoders/load_mfbacks.cpp
  src/decoders/smal.cpp
  src/decoders/unpack_thumb.cpp
  src/decoders/unpack.cpp
  src/demosaic/aahd_demosaic.cpp
  src/demosaic/ahd_demosaic.cpp
  src/demosaic/dcb_demosaic.cpp
  src/demosaic/dht_demosaic.cpp
  src/demosaic/misc_demosaic.cpp
  src/demosaic/xtrans_demosaic.cpp
  src/integration/dngsdk_glue.cpp
  src/integration/rawspeed_glue.cpp
  src/metadata/adobepano.cpp
  src/metadata/canon.cpp
  src/metadata/ciff.cpp
  src/metadata/cr3_parser.cpp
  src/metadata/epson.cpp
  src/metadata/exif_gps.cpp
  src/metadata/fuji.cpp
  src/metadata/identify_tools.cpp
  src/metadata/identify.cpp
  src/metadata/kodak.cpp
  src/metadata/leica.cpp
  src/metadata/makernotes.cpp
  src/metadata/mediumformat.cpp
  src/metadata/minolta.cpp
  src/metadata/misc_parsers.cpp
  src/metadata/nikon.cpp
  src/metadata/normalize_model.cpp
  src/metadata/olympus.cpp
  src/metadata/hasselblad_model.cpp
  src/metadata/p1.cpp
  src/metadata/pentax.cpp
  src/metadata/samsung.cpp
  src/metadata/sony.cpp
  src/metadata/tiff.cpp
  src/postprocessing/aspect_ratio.cpp
  src/postprocessing/dcraw_process.cpp
  src/postprocessing/mem_image.cpp
  src/postprocessing/postprocessing_aux.cpp
  src/postprocessing/postprocessing_utils_dcrdefs.cpp
  src/postprocessing/postprocessing_utils.cpp
  src/preprocessing/ext_preprocess.cpp
  src/preprocessing/raw2image.cpp
  src/preprocessing/subtract_black.cpp
  src/tables/cameralist.cpp
  src/tables/colorconst.cpp
  src/tables/colordata.cpp
  src/tables/wblists.cpp
  src/utils/curves.cpp
  src/utils/decoder_info.cpp
  src/utils/init_close_utils.cpp
  src/utils/open.cpp
  src/utils/phaseone_processing.cpp
  src/utils/read_utils.cpp
  src/utils/thumb_utils.cpp
  src/utils/utils_dcraw.cpp
  src/utils/utils_libraw.cpp
  src/write/apply_profile.cpp
  src/write/file_write.cpp
  src/write/tiff_writer.cpp
  src/x3f/x3f_parse_process.cpp
  src/x3f/x3f_utils_patched.cpp
)

target_include_directories(libraw
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libraw>
    $<INSTALL_INTERFACE:include/libraw>)
target_compile_definitions(libraw
  PUBLIC LIBRAW_NODLL
  PRIVATE LIBRAW_BUILDLIB USE_JPEG USE_JPEG8 USE_LCMS2 USE_ZLIB)
if(WIN32)
  target_compile_definitions(libraw PRIVATE UNICODE)
  target_link_libraries(libraw PUBLIC ws2_32)
endif()
if(MSVC)
  target_compile_definitions(libraw PRIVATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE)
endif()
target_link_libraries(libraw PUBLIC JPEG::JPEG lcms2::lcms2 ZLIB::ZLIB Threads::Threads)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/libraw-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/libraw-config.cmake
  "include(CMakeFindDependencyMacro)\n"
  "find_dependency(Threads)\n"
  "find_dependency(JPEG)\n"
  "find_dependency(lcms2 CONFIG)\n"
  "find_dependency(ZLIB)\n"
  "include(\"\${CMAKE_CURRENT_LIST_DIR}/libraw.cmake\")\n")

install(TARGETS libraw EXPORT libraw ARCHIVE DESTINATION lib)
install(DIRECTORY libraw DESTINATION include FILES_MATCHING PATTERN "*.h")
install(EXPORT libraw FILE libraw.cmake NAMESPACE libraw:: DESTINATION lib/cmake/libraw)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/libraw-config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/libraw-config-version.cmake
  DESTINATION lib/cmake/libraw)
