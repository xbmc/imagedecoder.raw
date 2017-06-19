project(imagedecoder.raw)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(Kodi REQUIRED)
find_package(RAW REQUIRED)

include_directories(${KODI_INCLUDE_DIR}/.. # Hack way with "/..", need bigger Kodi cmake rework to match right include ways (becomes done in future)
                    ${RAW_INCLUDE_DIRS})

set(RAW_SOURCES src/RawPicture.cpp)

if(WIN32)
  add_definitions(-DLIBRAW_NODLL)
endif()

set(DEPLIBS ${RAW_LIBRARIES})

build_addon(imagedecoder.raw RAW DEPLIBS)

include(CPack)
