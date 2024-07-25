cmake_minimum_required(VERSION 3.5)

project(pymapping LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS REQUIRED Core Quick Multimedia Positioning Sensors WebSockets)
if (Qt6Core_VERSION VERSION_LESS 6.5.1)
  message(FATAL_ERROR "This version of the ArcGIS Maps SDK for Qt requires at least Qt 6.5.1")
endif()

find_package(ArcGISRuntime 200.2.0 COMPONENTS REQUIRED Cpp)

# TODO: Change this path to your local vcpkg repository
#include($ENV{vcpkg_root}/scripts/buildsystems/vcpkg.cmake)

# TODO: Install pybind11 e.g. vcpkg install pybind11:x64-linux
find_package (pybind11 CONFIG REQUIRED)

pybind11_add_module (
    coremapping
    main.cpp
    MapViewModel.h
    MapViewModel.cpp
    SimpleGeoJsonLayer.h
    SimpleGeoJsonLayer.cpp
    GraphicsFactory.h
    GraphicsFactory.cpp
)

# Copy required dynamic libraries to the build folder as a post-build step.
if(DEFINED ArcGISRuntime_LIBRARIES)
  add_custom_command(TARGET coremapping POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${ArcGISRuntime_LIBRARIES}
    $<TARGET_FILE_DIR:coremapping>)
endif()

target_compile_definitions(coremapping
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(coremapping PRIVATE
  Qt6::Core
  Qt6::Quick
  Qt6::Multimedia
  Qt6::Positioning
  Qt6::Sensors
  Qt6::WebSockets
  ArcGISRuntime::Cpp)
