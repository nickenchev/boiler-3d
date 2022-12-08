cmake_minimum_required(VERSION 3.2)
project(boiler3d CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)

add_subdirectory(boiler/ libboiler)

add_executable(boiler3d
  src/main.cpp
  src/paddlemovementsystem.h
  src/paddlemovementsystem.cpp
  src/damagedealersystem.h
  src/damagedealersystem.cpp
  src/damagesystem.h
  src/damagesystem.cpp
  src/scorecomponent.h
  src/scoringsystem.h
  src/scoringsystem.cpp
  src/damagedealercomponent.h
  src/damagecomponent.h
  src/extralivescomponent.h
  src/multiplelifesystem.h
  src/multiplelifesystem.cpp
  src/samplepart.h
  src/samplepart.cpp)

target_include_directories(boiler3d PRIVATE boiler)
target_link_libraries(boiler3d boiler)
