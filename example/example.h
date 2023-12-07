//
// Created by CEO on 2023/12/7.
//

#ifndef MAIN_PS_EXAMPLE_H
#define MAIN_PS_EXAMPLE_H

#include "../Beidou/NMEA_0183_lib.h"

// GGA 4.1x
typedef struct GGA_Body_Raw {
    // 1. 时间
    // 10位 hhmmss.sss，时分秒格式
    u8 UTC_TIME[10];
    // 2. 纬度
    // 13位 ddmm.mmmmmmmm，度分格式（前导位数不足则补 0）
    // 北斗标准格式为llll.ll
    u8 Lat[15];
    // 3. 纬度标识
    // 1位 纬度 N（北纬）或 S（南纬）
    u8 Lat_dir[2];
    // 4. 经度
    // 14位 dddmm.mmmmmmmm，度分格式（前导位数不足则补 0）
    // 北斗标准格式为yyyyy.yy
    u8 Lon[16];
    // 5. 经度标识
    // 1位 经度 E（东经）或 W（西经）
    u8 Lon_dir[2];
    // 6. 定位质量
    // 2位 最后一位位\0 0：未定位，1：非差分定位，2：差分定位，3：无效PPS，6：正在估算
    u8 Pos_qua[2];
    // 7. 当前定位卫星数目
    // 3位 0 ~ 24  最后一位位\0
    u8 Satellites_num[3];
    // 8. HDOP水平精度因子（0.5 - 99.9）
    // 5位 最后一位位\0
    u8 HDOP[5];
    // 9. 海拔高度（-9999.9 - 99999.9）
    // 9位 最后一位位\0
    u8 Altitude[9];
    // 10. 高度单位米， 固定字符 M
    // fixed
    // 11. 地球椭球面相对大地水准面的高度
    // 9位 最后一位位\0
    u8 Height[9];
    // 12. 高度单位， 米， 固定字符 M
    // fixed
    // 13. 差分数据龄期
    u8 Age[5];
    // 14. 差分参考站的 ID
    u8 Ref_station_id[3];
    // 15. VDOP值
    u8 VDOP[5];
    // 16. 校验位，“$”与“*”之间字符异或以16进制显示
    u8 Check[2];
} GGA_Body_Raw;

// RMC 4.1x
typedef struct RMC_Body_Raw {
    // 1. 时间
    // 10位 hhmmss.sss，时分秒格式
    u8 UTC_TIME[10];
    // 2. 定位有效性标识，
    // 1位 A：定位有效，V：定位无效
    u8 Pos_validity[1];
    // 2. 纬度
    // 13位 ddmm.mmmmmmmm，度分格式（前导位数不足则补 0）
    // 北斗标准格式为llll.ll
    u8 Lat[13];
    // 3. 纬度标识
    // 1位 纬度 N（北纬）或 S（南纬）
    u8 Lat_dir[1];
    // 4. 经度
    // 14位 dddmm.mmmmmmmm，度分格式（前导位数不足则补 0）
    // 北斗标准格式为yyyyy.yy
    u8 Lon[14];
    // 5. 经度标识
    // 1位 经度 E（东经）或 W（西经）
    u8 Lon_dir[1];
    // 7. 对地速度 eg: 0.003
    // 5位 最后一位位\0
    u8 Gro_speed[6];
    // 8. 对地航向 eg: 114.8
    // 5位 最后一位位\0
    u8 Gro_dir[6];
    // 9. UTC日期
    // 6位 ddmmyy 日月年格式
    u8 UTC_DATE[6];
    // 10. 磁偏角
    // 4位 最后一位位\0
    u8 Magnetic_dec[4];
    // 11. 磁偏角方向
    // 1位 磁偏角方向： E：东， W：西。
    u8 Magnetic_dec_dir[1];
    // 12. 定位模式标志
    // 1位 定位模式标志：A：自主定位E：估算模式N：数据无效D：差分模式M：未定位
    u8 Pos_mode_flag[1];
    // 13. 导航状态标识， V表示系统不输出导航状态信息（仅NMEA 4.1以上有效）
    u8 Nav_status[2];
    // 14. 校验位
    u8 Check[2];
} RMC_Body_Raw;

static const u8 prn_len = 3;
static const u8 Elevation_len = 5;
static const u8 Azimuth_len = 6;
static const u8 SNR_len = 5;
// GSV 4.1x
typedef struct GSV_Body_Raw {
    // 1. 本次 GSV 语句的总数目
    // 1位 1 - 3
    u8 GSV_num[2];
    // 2. 本条 GSV 语句是本次 GSV 语句的第几条
    // 1位 1 - 3
    u8 Index[2];
    // 3. 当前可见卫星总数（前导位数不足则补 0）
    // 2位 00 - 12
    u8 Satellites_num[3];

    // 4. PRN 码（伪随机噪声码）（前导位数不足则补 0）
    // 2位 01 - 32
    u8 PRN_1[prn_len];
    // 5. 卫星仰角（前导位数不足则补 0）
    // 2位 00 - 90
    u8 Elevation_1[Elevation_len];
    // 6. 卫星方位角（前导位数不足则补 0）
    // 3位 00 - 359
    u8 Azimuth_1[Azimuth_len];
    // 7. 信噪比
    // 2位 00 - 99 dbHz
    u8 SNR_1[SNR_len];

    // 8. PRN 码（伪随机噪声码）（前导位数不足则补 0）
    // 2位 01 - 32
    u8 PRN_2[prn_len];
    // 9. 卫星仰角（前导位数不足则补 0）
    // 2位 00 - 90
    u8 Elevation_2[Elevation_len];
    // 10. 卫星方位角（前导位数不足则补 0）
    // 3位 00 - 359
    u8 Azimuth_2[Azimuth_len];
    // 11. 信噪比
    // 2位 00 - 99 dbHz
    u8 SNR_2[SNR_len];

    // 12. PRN 码（伪随机噪声码）（前导位数不足则补 0）
    // 2位 01 - 32
    u8 PRN_3[prn_len];
    // 13. 卫星仰角（前导位数不足则补 0）
    // 2位 00 - 90
    u8 Elevation_3[Elevation_len];
    // 14. 卫星方位角（前导位数不足则补 0）
    // 3位 00 - 359
    u8 Azimuth_3[Azimuth_len];
    // 15. 信噪比
    // 2位 00 - 99 dbHz
    u8 SNR_3[SNR_len];

    // 16. PRN 码（伪随机噪声码）（前导位数不足则补 0）
    // 2位 01 - 32
    u8 PRN_4[prn_len];
    // 17. 卫星仰角（前导位数不足则补 0）
    // 2位 00 - 90
    u8 Elevation_4[Elevation_len];
    // 18. 卫星方位角（前导位数不足则补 0）
    // 3位 00 - 359
    u8 Azimuth_4[Azimuth_len];
    // 19. 信噪比
    // 2位 00 - 99 dbHz
    u8 SNR_4[SNR_len];

//    // 20. GNSS信号ID, 	0 代表全部信号，仅 NMEA 4.1 及以上版本有效
//    u8 GNSS_signal_id[5];
    // 21. 校验位
    u8 Check[2];
} GSV_Body_Raw;


// LSP
typedef struct LPS_Body_Raw {
    // 1. 闰秒状态
    // 11B，sppffffffff
    // s：闰秒状态操作，0=无操作，1=正闰秒，2=负闰秒
    // pp：BD 时对应的闰秒，2B，I1，单位 s，BD 时-UTC
    u8 lss[12];
    // 2. 闰秒发生时分秒
    // 6B，hhmmss
    // hh：闰秒发生时，00~23
    // mm：闰秒发生分，00~59
    // ss：闰秒发生秒，00~60，60=正闰秒，58=负闰秒
    u8 lss_time[7];
    // 3. 闰秒发生日
    // 2B 01~31
    u8 lss_day[3];
    // 4. 闰秒发生月
    // 2B 01~12
    u8 lss_month[3];
    // 5. 闰秒发生年
    // 4B yyyy
    u8 lss_year[5];
    // 6. 校验位
    u8 check[2];
} LPS_Body_Raw;

// ZDA
typedef struct ZDA_Body_Raw {
    // 1. 模式指示
    // 1-RDSS定时结果、2-RNSS定时结果
    u8 mode[2];
    // 2. UTC 时间
    u8 UTC[10];
    // 3. 日     00～31
    u8 day[3];
    // 4. 月	 00～12
    u8 month[3];
    // 5. 年	>2000
    u8 year[5];
    // 6. 本地时区	00～±13
    u8 local_zone[3];
    // 7. 本时区分钟差	00～59
    u8 minute_diff[3];
    // 8. 定时修正值时刻
    u8 time_corr[10];
    // 9. 修正值
    u8 time_corr_value[3];
    // 10. 精度指示
    u8 accuracy[3];
    // 11. 卫星信号锁定状态
    u8 lock[2];
    // 12. 校验位
    u8 check[2];
} ZDA_Body_Raw;

// CSM
typedef struct CSM_Body_Raw {
    // 1. 当前残差
    u8 csm[16];
    // 2. 校验码
    u8 check[2];
} CSM_Body_Raw;


// 解析报文函数

u16 GGA_data_get(GGA_Body_Raw *GGA, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index);
u16 RMC_data_get(RMC_Body_Raw *RMC, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index);
u16 GSV_data_get(GSV_Body_Raw *GSV, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index);
u16 CSM_data_get(CSM_Body_Raw *CSM, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index);
u16 LPS_data_get(LPS_Body_Raw *LPS, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index);
u16 ZDA_data_get(ZDA_Body_Raw *ZDA, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index);


#endif //MAIN_PS_EXAMPLE_H
