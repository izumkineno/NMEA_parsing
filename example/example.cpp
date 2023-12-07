//
// Created by CEO on 2023/12/7.
//

#include "example.h"


// 搜寻包
u16 GGA_data_get(GGA_Body_Raw *GGA, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index) {
    // 1位，固定M，10位是防止接收到的错误的卫星数据导致切片错误
    u8 t[10] = {0};
    // 建立卫星结构体指针数组
    u8 *array[] = {
            GGA->UTC_TIME,
            GGA->Lat,
            GGA->Lat_dir,
            GGA->Lon,
            GGA->Lon_dir,
            GGA->Pos_qua,
            GGA->Satellites_num,
            GGA->HDOP,
            GGA->Altitude,
            t,
            GGA->Height,
            t,
            GGA->Age,
            GGA->Ref_station_id,
            GGA->VDOP,
            GGA->Check,
    };
    // 协议头
    u8 header[7] = "$BDGGA";
    u8 header_len = sizeof (header) / sizeof (header[0]) - 1;
    // 调用模板
    return template_body_parse(buffer, index, type, array, sizeof (array) / sizeof (array[0]), sizeof (array), start_index, header_state, header, header_len);
}

u16 RMC_data_get(RMC_Body_Raw *RMC, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index) {
    u8 *array[] = {
            RMC->UTC_TIME,
            RMC->Pos_validity,
            RMC->Lat,
            RMC->Lat_dir,
            RMC->Lon,
            RMC->Lon_dir,
            RMC->Gro_speed,
            RMC->Gro_dir,
            RMC->UTC_DATE,
            RMC->Magnetic_dec,
            RMC->Magnetic_dec_dir,
            RMC->Pos_mode_flag,
            RMC->Nav_status,
            RMC->Check,
    };
    u8 header[7] = "$BDRMC";
    u8 header_len = sizeof (header) / sizeof (header[0]) - 1;

    return template_body_parse(buffer, index, type, array, sizeof (array) / sizeof (array[0]), sizeof (array), start_index, header_state, header, header_len);
}

u16 GSV_data_get(GSV_Body_Raw *GSV, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index) {
    u8 *array[] = {
            GSV->GSV_num,
            GSV->Index,
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
//            GSV->GNSS_signal_id,
            GSV->Check,
    };
    u8 header[7] = "$BDGSV";
    u8 header_len = sizeof (header) / sizeof (header[0]) - 1;

    return template_body_parse(buffer, index, type, array, sizeof (array) / sizeof (array[0]), sizeof (array), start_index, header_state, header, header_len);
}

u16 CSM_data_get(CSM_Body_Raw *CSM, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index) {
    u8 *array[] = {
            CSM->csm,
            CSM->check,
    };
    u8 header[7] = "$PGCSM";

    u8 header_len = sizeof (header) / sizeof (header[0]) - 1;

    return template_body_parse(buffer, index, type, array, sizeof (array) / sizeof (array[0]), sizeof (array), start_index, header_state, header, header_len);
}

u16 LPS_data_get(LPS_Body_Raw *LPS, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index) {
    u8 *array[] = {
            LPS->lss,
            LPS->lss_time,
            LPS->lss_day,
            LPS->lss_month,
            LPS->lss_year,
            LPS->check,
    };
    u8 header[7] = "$BDLPS";
    u8 header_len = sizeof (header) / sizeof (header[0]) - 1;

    return template_body_parse(buffer, index, type, array, sizeof (array) / sizeof (array[0]), sizeof (array), start_index, header_state, header, header_len);
}

u16 ZDA_data_get(ZDA_Body_Raw *ZDA, u8 *buffer, u16 index, Type type, u8 *header_state, u16 *start_index) {
    u8 *array[] = {
            ZDA->mode,
            ZDA->UTC,
            ZDA->day,
            ZDA->month,
            ZDA->year,
            ZDA->local_zone,
            ZDA->minute_diff,
            ZDA->time_corr,
            ZDA->time_corr_value,
            ZDA->accuracy,
            ZDA->lock,
            ZDA->check,
    };
    u8 header[7] = "$BDZDA";
    u8 header_len = sizeof (header) / sizeof (header[0]) - 1;

    return template_body_parse(buffer, index, type, array, sizeof (array) / sizeof (array[0]), sizeof (array), start_index, header_state, header, header_len);
}

