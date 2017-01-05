project(imagedecoder.raw)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(Kodi REQUIRED)
find_package(RAW REQUIRED)

include_directories(${KODI_INCLUDE_DIR}
                    ${RAW_INCLUDE_DIRS})

set(RAW_SOURCES src/RawPicture.cpp)

set(DEPLIBS ${RAW_LIBRARIES})

build_addon(imagedecoder.raw RAW DEPLIBS)

include(CPack)
