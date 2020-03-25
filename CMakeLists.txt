cmake_minimum_required(VERSION 3.5)
project(imagedecoder.raw)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

find_package(Kodi REQUIRED)
find_package(JPEG REQUIRED)
find_package(LCMS2 REQUIRED)
find_package(RAW REQUIRED)

include_directories(${KODI_INCLUDE_DIR}/.. # Hack way with "/..", need bigger Kodi cmake rework to match right include ways (becomes done in future)
                    ${LCMS2_INCLUDE_DIRS}
                    ${RAW_INCLUDE_DIRS})

set(RAW_SOURCES src/RawPicture.cpp)

if(WIN32)
  add_definitions(-DLIBRAW_NODLL)
endif()

set(DEPLIBS ${RAW_LIBRARIES}
            ${LCMS2_LIBRARIES}
            ${JPEG_LIBRARIES})

build_addon(imagedecoder.raw RAW DEPLIBS)

include(CPack)
