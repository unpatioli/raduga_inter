cmake_minimum_required(VERSION 3.6)
project(interpolation)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Charts REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(interpolation ${SOURCE_FILES})

qt5_use_modules(interpolation Core Widgets Gui Charts)