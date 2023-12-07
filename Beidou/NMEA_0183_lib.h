//
// Created by izum on 2023/11/1.
//

#ifndef LIB_NMEA_0183_NMEA_0183_LIB_H
#define LIB_NMEA_0183_NMEA_0183_LIB_H
#include "../type.h"

typedef enum {
    GP, // GPS
    BD, // 北斗
    GL, // GLONASS
    ALL,// 忽略头部
} Type;

u16 template_body_parse(u8 *buffer, u16 index,Type type, u8 **array, u16 array_len, u16 array_size, u16 *start_index,u8 *header_state, u8 *header, u8 header_len);

#endif //LIB_NMEA_0183_NMEA_0183_LIB_H
