cmake_minimum_required(VERSION 3.26)
project(lib_NMEA_0183 C)

set(CMAKE_C_STANDARD 99)

add_executable(lib_NMEA_0183 main.c
        c/NMEA_0183_lib.c
        c/NMEA_0183_lib.h
        data_target_RS485.h)
