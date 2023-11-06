//
// Created by izum on 2023/11/1.
//

#ifndef LIB_NMEA_0183_NMEA_0183_LIB_H
#define LIB_NMEA_0183_NMEA_0183_LIB_H

typedef enum {
    GP, // GPS
    BD, // 北斗
    GL, // GLONASS
    ALL,// 忽略头部
} Type;

// GGA 4.1x
typedef struct GGA_Body_Raw {
    // 1. 时间
    // 10位 hhmmss.sss，时分秒格式
    uint8_t UTC_TIME[10];
    // 2. 纬度
    // 13位 ddmm.mmmmmmmm，度分格式（前导位数不足则补 0）
    // 北斗标准格式为llll.ll
    uint8_t Lat[13];
    // 3. 纬度标识
    // 1位 纬度 N（北纬）或 S（南纬）
    uint8_t Lat_dir;
    // 4. 经度
    // 14位 dddmm.mmmmmmmm，度分格式（前导位数不足则补 0）
    // 北斗标准格式为yyyyy.yy
    uint8_t Lon[14];
    // 5. 经度标识
    // 1位 经度 E（东经）或 W（西经）
    uint8_t Lon_dir;
    // 6. 定位质量
    // 2位 最后一位位\0 0：未定位，1：非差分定位，2：差分定位，3：无效PPS，6：正在估算
    uint8_t Pos_qua[2];
    // 7. 当前定位卫星数目
    // 3位 0 ~ 24  最后一位位\0
    uint8_t Satellites_num[3];
    // 8. HDOP水平精度因子（0.5 - 99.9）
    // 5位 最后一位位\0
    uint8_t HDOP[5];
    // 9. 海拔高度（-9999.9 - 99999.9）
    // 9位 最后一位位\0
    uint8_t Altitude[9];
    // 10. 高度单位米， 固定字符 M
    // fixed
    // 11. 地球椭球面相对大地水准面的高度
    // 9位 最后一位位\0
    uint8_t Height[9];
    // 12. 高度单位， 米， 固定字符 M
    // fixed
    // 13. V表示系统不输出导航状态信息（仅NMEA 4.1以上有效）
    // empty
    // todo 分开 * 左右两个数据
    // 14. 差分参考站的 ID
    // 15. 校验位，“$”与“*”之间字符异或以16进制显示
    uint8_t Check[8];
} GGA_Body_Raw;

// RMC 4.1x
typedef struct RMC_Body_Raw {
    // 1. 时间
    // 10位 hhmmss.sss，时分秒格式
    uint8_t UTC_TIME[10];
    // 2. 定位有效性标识，
    // 1位 A：定位有效，V：定位无效
    uint8_t Pos_validity;
    // 2. 纬度
    // 13位 ddmm.mmmmmmmm，度分格式（前导位数不足则补 0）
    // 北斗标准格式为llll.ll
    uint8_t Lat[13];
    // 3. 纬度标识
    // 1位 纬度 N（北纬）或 S（南纬）
    uint8_t Lat_dir;
    // 4. 经度
    // 14位 dddmm.mmmmmmmm，度分格式（前导位数不足则补 0）
    // 北斗标准格式为yyyyy.yy
    uint8_t Lon[14];
    // 5. 经度标识
    // 1位 经度 E（东经）或 W（西经）
    uint8_t Lon_dir;
    // 7. 对地速度 eg: 0.003
    // 5位 最后一位位\0
    uint8_t Gro_speed[6];
    // 8. 对地航向 eg: 114.8
    // 5位 最后一位位\0
    uint8_t Gro_dir[6];
    // 9. UTC日期
    // 6位 ddmmyy 日月年格式
    uint8_t UTC_DATE[6];
    // 10. 磁偏角
    // 4位 最后一位位\0
    uint8_t Magnetic_dec[4];
    // 11. 磁偏角方向
    // 1位 磁偏角方向： E：东， W：西。
    uint8_t Magnetic_dec_dir;
    // 12. 定位模式标志
    // 1位 定位模式标志：A：自主定位E：估算模式N：数据无效D：差分模式M：未定位
    uint8_t Pos_mode_flag;
    // todo 分开 * 左右两个数据
    // 13. 导航状态标识， V表示系统不输出导航状态信息（仅NMEA 4.1以上有效）
    uint8_t Nav_status;
    // 14. 校验位
    uint8_t Check[4];
} RMC_Body_Raw;

// GSV 4.1x
typedef struct GSV_Body_Raw {
    // 1. 本次 GSV 语句的总数目
    // 1位 1 - 3
    uint8_t GSV_num;
    // 2. 本条 GSV 语句是本次 GSV 语句的第几条
    // 1位 1 - 3
    uint8_t Index;
    // 3. 当前可见卫星总数（前导位数不足则补 0）
    // 2位 00 - 12
    uint8_t Satellites_num[2];

    // 4. PRN 码（伪随机噪声码）（前导位数不足则补 0）
    // 2位 01 - 32
    uint8_t PRN_1[2];
    // 5. 卫星仰角（前导位数不足则补 0）
    // 2位 00 - 90
    uint8_t Elevation_1[2];
    // 6. 卫星方位角（前导位数不足则补 0）
    // 3位 00 - 359
    uint8_t Azimuth_1[3];
    // 7. 信噪比
    // 2位 00 - 99 dbHz
    uint8_t SNR_1[2];

    // 8. PRN 码（伪随机噪声码）（前导位数不足则补 0）
    // 2位 01 - 32
    uint8_t PRN_2[2];
    // 9. 卫星仰角（前导位数不足则补 0）
    // 2位 00 - 90
    uint8_t Elevation_2[2];
    // 10. 卫星方位角（前导位数不足则补 0）
    // 3位 00 - 359
    uint8_t Azimuth_2[3];
    // 11. 信噪比
    // 2位 00 - 99 dbHz
    uint8_t SNR_2[2];

    // 12. PRN 码（伪随机噪声码）（前导位数不足则补 0）
    // 2位 01 - 32
    uint8_t PRN_3[2];
    // 13. 卫星仰角（前导位数不足则补 0）
    // 2位 00 - 90
    uint8_t Elevation_3[2];
    // 14. 卫星方位角（前导位数不足则补 0）
    // 3位 00 - 359
    uint8_t Azimuth_3[3];
    // 15. 信噪比
    // 2位 00 - 99 dbHz
    uint8_t SNR_3[2];

    // 16. PRN 码（伪随机噪声码）（前导位数不足则补 0）
    // 2位 01 - 32
    uint8_t PRN_4[2];
    // 17. 卫星仰角（前导位数不足则补 0）
    // 2位 00 - 90
    uint8_t Elevation_4[3];
    // 18. 卫星方位角（前导位数不足则补 0）
    // 3位 00 - 359
    uint8_t Azimuth_4[2];
    // 19. 信噪比
    // 2位 00 - 99 dbHz
    uint8_t SNR_4[2];

    // todo 分开 * 左右两个数据
    // 20. GNSS信号ID, 	0 代表全部信号，仅 NMEA 4.1 及以上版本有效
    uint8_t GNSS_signal_id;
    // 21. 校验位 4位
    uint8_t Check[5];
} GSV_Body_Raw;

// 通用解析函数

void get_AABBCC(const uint8_t* aabbcc, uint8_t* a, uint8_t* b, uint8_t* c);

double get_lat_lon(uint8_t *data_raw, bool is_lat);

// 解析报文函数

int GGA_data_get(uint8_t *buffer, GGA_Body_Raw *GGA, Type type);

int RMC_data_get(uint8_t *buffer, RMC_Body_Raw *RMC, Type type);

int GSV_data_get(uint8_t *buffer, GSV_Body_Raw *GSV, Type type);

#endif //LIB_NMEA_0183_NMEA_0183_LIB_H
