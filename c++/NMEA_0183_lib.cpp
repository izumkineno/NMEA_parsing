//
// Created by izum on 2023/11/1.
//

#include <cstdint>
#include <cstdlib>

#include "NMEA_0183_lib.h"

/**
 * 设置解析数据头
 * @param header 字头字符串 eg：$GPRMC
 * @param type 字头字符串类型
 */
void set_header_type(uint8_t *header, Type type) {
    switch (type) {
        case ALL:
            header[1] = ' ';
            header[2] = ' ';
            break;
        case GP:
            header[1] = 'G';
            header[2] = 'P';
            break;
        case BD:
            header[1] = 'B';
            header[2] = 'D';
            break;
        case GL:
            header[1] = 'G';
            header[2] = 'L';
            break;
    }
}

/**
 * 头部状态机
 * @param buf 待检测的字符串缓冲区
 * @param header 目标头部字符串
 * @param header_len 目标头部字符串长度
 * @param state 当前检测到的位置
 * @param type 头部字符串的类型
 * @return 1：检测到，0：未检测到
 */
bool check_header(uint8_t buf, uint8_t *header, uint8_t header_len, uint8_t *state, Type type) {
    // 设置头部字符串的类型（可能是部分匹配或全部匹配）
    set_header_type(header, type);

    if (type == ALL && (*state == 1 || *state == 2)) {
        *state += 1; // 全部匹配模式中，前两个字符不用检查，直接匹配成功
    } else {
        if (buf == header[*state]) {
            *state += 1; // 匹配成功，继续检查下一个字符
        } else {
            *state = 0; // 匹配失败，重置状态
        }
    }

    return *state == header_len; // 检查是否完全匹配头部字符串
}


/**
 * 配合头部状态检查本体携带的数据，检测到一包后会跳出并返回跳出位置
 * @param buffer 待检测的字符串缓冲区
 * @param vec_data body结构体按顺序组成的数组
 * @param vec_data_len body结构体按顺序组成的数组长度
 * @param vec_data_size body数组的大小，用于判定溢出
 *
 * @param header 目标头部字符串
 * @param header_len 目标头部字符串长度
 * @param state 当前检测到的位置
 * @param type 头部字符串的类型
 *
 * @param body_flag 检测到完整头部的标志位
 * @param last_coma_index 上一个逗号出现的index
 * @param coma_counter 出现逗号的总次数
 * @return 0：失败，X：在buffer的x位置检测完成
 */
int check_body(const uint8_t *buffer, uint8_t *vec_data[], uint8_t vec_data_len, int vec_data_size,
               uint8_t *header, uint8_t header_len, uint8_t *state, Type type,
               bool *body_flag, uint16_t *last_coma_index, uint8_t *coma_counter) {
    int recv_len = 0;    // 接收数据的总长度
    int start_index = 0; // 开始解析的位置

    // 遍历输入缓冲区
    for (int i = 0; buffer[i] != '\0'; ++i) {
        uint8_t c = buffer[i];

        if (*body_flag) {
            // todo 分开 * 左右两个数据
            // 已找到头部，解析数据
            if (c == '\n') {
                // 在'\n'字符处结束一包数据解析
                for (int j = 1; j < i - *last_coma_index; ++j) {
                    vec_data[vec_data_len - 1][j - 1] = buffer[*last_coma_index + j];
                }
                return i;
            } else if (c == ',') {
                // 逗号分隔符，用于分割数据字段
                (*coma_counter)++;
                if (*coma_counter >= 2) {
                    uint8_t index = *coma_counter - 2;
                    int len = i - *last_coma_index;
                    recv_len += len;

                    // 检查数据是否超出大小
                    if (recv_len < vec_data_size) {
                        for (int j = 1; j < len; ++j) {
                            vec_data[index][j - 1] = buffer[*last_coma_index + j];
                        }
                    } else {
                        return 0; // 数据溢出
                    }
                }
                *last_coma_index = i;
            } else if (*coma_counter > vec_data_len || (i - start_index) > vec_data_size) {
                return 0; // 数据溢出或字段数超出
            }
        } else {
            // 未找到头部，检查头部字符
            if (check_header(c, header, header_len, state, type)) {
                *body_flag = true;
                *state = 0;
                start_index = i;
            }
        }
    }

    return 0; // 未找到完整数据包
}


/**
 * 分离aabbcc格式数据
 * @param aabbcc
 * @param a
 * @param b
 * @param c
 */
void get_AABBCC(const uint8_t* aabbcc, uint8_t* a, uint8_t* b, uint8_t* c) {
    *a = (aabbcc[0] - '0') * 10 + (aabbcc[1] - '0');
    *b = (aabbcc[2] - '0') * 10 + (aabbcc[3] - '0');
    *c = (aabbcc[4] - '0') * 10 + (aabbcc[5] - '0');
}

/**
 * 计算经纬度
 * @param data_raw 原始数据
 * @param is_lat 1表示经度lat，0表示纬度lon
 * @return 浮点数的真实数据
 */
double get_lat_lon(uint8_t *data_raw, bool is_lat) {
    uint8_t *t1, *t2;
    if (is_lat) {
        uint8_t t[] = {data_raw[0], data_raw[1], '\0'};
        t1 = t;
        t2 = data_raw + 2;
    } else {
        uint8_t t[] = {data_raw[0], data_raw[1], data_raw[2], '\0'};
        t1 = t;
        t2 = data_raw + 3;
    }

    double num = atoi((const char *) t1) + atof((const char *) t2) / 60;
    return num;
}

// 搜寻一包GGA
int GGA_data_get(uint8_t *buffer, GGA_Body_Raw *GGA, Type type) {
    uint8_t t[1] = {0};

    uint8_t *vec_GGA[] = {
            GGA->UTC_TIME,
            GGA->Lat,
            &GGA->Lat_dir,
            GGA->Lon,
            &GGA->Lon_dir,
            GGA->Pos_qua,
            GGA->Satellites_num,
            GGA->HDOP,
            GGA->Altitude,
            t,
            GGA->Height,
            t,
            t,
            GGA->Check,
    };

    uint8_t header_len = 6;
    uint8_t header[6] = {'$', 'B', 'D', 'G', 'G', 'A'};
    uint8_t state = 0;
    bool body_flag = false;
    uint16_t last_coma_index = 0;
    uint8_t coma_counter = 0;

    // GGA max length 72 + 12 标准最大字长GGA字长为 82
    int i = check_body(buffer, vec_GGA, 14, 84, header, header_len, &state, type, &body_flag, &last_coma_index,&coma_counter);
    return i;
}

// 搜寻一包RMC
int RMC_data_get(uint8_t *buffer, RMC_Body_Raw *RMC, Type type) {
    uint8_t *vec_RMC[] = {
            RMC->UTC_TIME,
            &RMC->Pos_validity,
            RMC->Lat,
            &RMC->Lat_dir,
            RMC->Lon,
            &RMC->Lon_dir,
            RMC->Gro_speed,
            RMC->Gro_dir,
            RMC->UTC_DATE,
            RMC->Magnetic_dec,
            &RMC->Magnetic_dec_dir,
            &RMC->Pos_mode_flag,
            &RMC->Nav_status,
            RMC->Check,
    };
    uint8_t header_len = 6;
    uint8_t header[6] = {'$', 'B', 'D', 'R', 'M', 'C'};
    uint8_t state = 0;
    bool body_flag = false;
    uint16_t last_coma_index = 0;
    uint8_t coma_counter = 0;

    // RMC max length 70 + 12
    int i = check_body(buffer, vec_RMC, 14, 82, header, header_len, &state, type, &body_flag, &last_coma_index, &coma_counter);
    return i;
}

// 搜寻一包GSV
int GSV_data_get(uint8_t *buffer, GSV_Body_Raw *GSV, Type type) {
    uint8_t *vec_GSV[] = {
            &GSV->GSV_num,
            &GSV->Index,
            GSV->Satellites_num,
            GSV->PRN_1,
            GSV->Elevation_1,
            GSV->Azimuth_1,
            GSV->SNR_1,
            GSV->PRN_2,
            GSV->Elevation_2,
            GSV->Azimuth_2,
            GSV->SNR_2,
            GSV->PRN_3,
            GSV->Elevation_3,
            GSV->Azimuth_3,
            GSV->SNR_3,
            GSV->PRN_4,
            GSV->Elevation_4,
            GSV->Azimuth_4,
            GSV->SNR_4,
            &GSV->GNSS_signal_id,
            GSV->Check,
    };

    uint8_t header_len = 6;
    uint8_t header[6] = {'$', 'B', 'D', 'G', 'S', 'V'};
    uint8_t state = 0;
    bool body_flag = false;
    uint16_t last_coma_index = 0;
    uint8_t coma_counter = 0;

    int i = check_body(buffer, vec_GSV, 21, sizeof(vec_GSV), header, header_len, &state, type, &body_flag, &last_coma_index, &coma_counter);
    return i;
}