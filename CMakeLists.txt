cmake_minimum_required(VERSION 3.2)
project(boiler3d CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)

add_subdirectory(boiler/ libboiler)

add_executable(boiler3d
  src/main.cpp
  src/samplepart.cpp)

target_include_directories(boiler3d INTERFACE boiler)
target_link_libraries(boiler3d boiler)
