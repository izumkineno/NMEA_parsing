cmake_minimum_required(VERSION 3.26)
project(main_PS)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS OFF)


add_executable(main_PS
        main.h
        main.cpp
        Beidou/NMEA_0183_lib.cpp
        example/example.cpp
        example/example.h
)

