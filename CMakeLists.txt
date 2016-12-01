cmake_minimum_required(VERSION 3.6)
project(interpolation)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Charts REQUIRED)

set(SOURCE_FILES main.cpp Interpolation/Interpolator.cpp Interpolation/Interpolator.h Interpolation/LinearInterpolator.cpp Interpolation/LinearInterpolator.h Interpolation/SquareInterpolator.cpp Interpolation/SquareInterpolator.h Interpolation/CubicSplineInterpolator.cpp Interpolation/CubicSplineInterpolator.h)
add_executable(interpolation ${SOURCE_FILES})

qt5_use_modules(interpolation Core Widgets Gui Charts)