//
// Created by izum on 2023/11/1.
//
#include <iostream>
#include "NMEA_0183_lib.h"

/**
 * 设置解析数据头
 * @param header 字头字符串 eg：$GPRMC
 * @param type 字头字符串类型
 */
void set_header_type(u8 *header, Type type) {
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
bool check_header(u8 buf, u8 *header, u8 header_len, u8 *state, Type type) {

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
 * @param start_index 检测到完整头部的位置 最大65535
 *
 * @param body_flag 检测到完整头部的标志位
 * @param last_coma_index 上一个逗号出现的index
 * @param coma_counter 出现逗号的总次数
 * @param check 已经计算过头部的校验码
 * @return 0：失败，X：在buffer的x位置检测完成
 */
u16 body_parse(const u8 *buffer, u8 *vec_data[], u8 vec_data_len, u16 vec_data_size,
               u16 start_index,
               const bool *body_flag, u16 *last_coma_index, u8 *coma_counter, u8* check) {
    // 遍历输入缓冲区
    if (*body_flag) {
        // 接收数据的总长度
        u16 recv_len = 0;
        for (u16 i = start_index; buffer[i] != '\0'; ++i) {
            u8 c = buffer[i];
            if (c == ',') {
                // 逗号分隔符，用于分割数据字段
                (*coma_counter)++;
                if (*coma_counter >= 2) {
                    u8 index = *coma_counter - 2;
                    u16 len = i - *last_coma_index;
                    recv_len += len;

                    // 检查数据是否超出大小
                    if (recv_len < vec_data_size) {
                        for (u16 j = 1; j < len; ++j) {
                            vec_data[index][j - 1] = buffer[*last_coma_index + j];
                        }
                    } else {
                        return 0; // 数据溢出
                    }
                }
                *last_coma_index = i;
            }
            else if (c == '*') {
                // 求总校验码
                for (u16 j = start_index + 1; j < i; ++j) {
                    u8 ch = buffer[j];
                    *check ^= ch;
                }
                (*coma_counter)++;
                for (u16 j = 1; j < i - *last_coma_index; ++j) {
                    vec_data[vec_data_len - 2][j - 1] = buffer[*last_coma_index + j];
                }
                *last_coma_index = i;
            }
            else if (c == '\n') {
                // todo remove
                std::cout << i - start_index - 2 << "--=--" << (int)vec_data_size << "-------------";
                // 不够长度检查校验码，就跳出
                if (i - start_index > vec_data_size + 2 || i - start_index < 0) {
                    return 0;
                }
                // 在'\n'字符处结束一包数据解析
                u8 check_char[2];
                for (u16 j = 1; j < i - *last_coma_index; ++j) {
                    u8 t = buffer[*last_coma_index + j];
                    vec_data[vec_data_len - 1][j - 1] = t;
                    check_char[j - 1] = t;
                }

                // 计算出的校验数转字符
                u8 check_char2[2];
                u8 n = *check & 0x0f;
                if (n < 0x0a) {
                    check_char2[0] = n + '0';
                } else {
                    check_char2[0] = n - 10 + 'A';
                }
                n = *check >> 4;
                if (n < 0x0a) {
                    check_char2[1] = n + '0';
                } else {
                    check_char2[1] = n - 10 + 'A';
                }

                if (!(check_char[0] == check_char2[1] && check_char[1] == check_char2[0])) {
                    // todo remove
                    std::cout << "check error" << std::endl;
                    return 0;
                }

                return i;
            }
            else if (*coma_counter > vec_data_len || (i - start_index) > vec_data_size + 1) {
                return 0; // 数据溢出或字段数超出
            }
        }
    }

    return 0; // 未找到完整数据包
}

/**
 * 整合了头部状态机和协议数据抽离的模板函数
 * @param buffer 待检测的字符串缓冲区
 * @param index 字符串缓冲区的位置
 * @param array 协议结构体属性指针数组
 * @param array_len 指针数组长度
 * @param array_size 指针数组大小
 * @param start_index 检测到完整头部的位置 最大65535
 * @param header 协议头数组
 * @param header_state 头部状态机位置
 * @param header_len 协议头部长度，当这个值等于状态机时，即检测成功
 * @return 0：失败，X：在buffer的x位置检测完成
 */
u16 template_body_parse(
        u8 *buffer, u16 index,
        Type type,
        u8 **array, u16 array_len, u16 array_size, u16 *start_index,
        u8 *header_state, u8 *header, u8 header_len
) {
    u8 buf = buffer[index];
    // 设置头部字符串的类型（可能是部分匹配或全部匹配）
    set_header_type(header, type);
    if (check_header(buf, header, header_len, header_state, type)) {
        bool body_ready = true;
        *header_state = 0;
        *start_index = index;
        // 求头部校验码
        u8 check = 0;
        for (u32 j = *start_index - header_len + 2; j < *start_index + 1; ++j) {
            check ^= buffer[j];
        }

        u16 last_coma_index = 0;
        u8 coma_counter = 0;

        u16 index_ = body_parse(buffer, array, array_len, array_size, *start_index, &body_ready, &last_coma_index, &coma_counter, &check);
        *start_index -= *start_index >= header_len ? header_len - 1 : *start_index;
        return index_;
    }
    return 0;
}

