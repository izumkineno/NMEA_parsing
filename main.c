#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "c/NMEA_0183_lib.h"
//#include "data_target_RS485.h"
//
//data_time dataTime = {0x02, {0}, 0, 0, {0}, {0}};
//data_ltd dataLtd = {0x03, 0, {0}, {0}, {0}, {0}, {0}, 0, 0};

void storeToByteArray(int value, uint8_t* byteArray, size_t size, int isBigEndian) {
    size_t i;
    if (isBigEndian) {
        for (i = 0; i < size; ++i) {
            byteArray[i] = (value >> (8 * (size - 1 - i))) & 0xFF;
        }
    } else {
        for (i = 0; i < size; ++i) {
            byteArray[i] = (value >> (8 * i)) & 0xFF;
        }
    }
}

int RMC_test(uint8_t *test) {
    RMC_Body_Raw RMC = {{0}, 0, {0}, 0, {0}, 0, {0}, {0}, {0}, {0}, 0, 0, 0, 0};

    int index = RMC_data_get(test, &RMC, ALL);

    if (index != 0) {
        printf("RMC: YES index is %d\n", index);

        uint8_t d,M,y;
        get_AABBCC(RMC.UTC_DATE, &d,&M,&y);
        printf("date: 20%d-%02d-%02d\n", y,M,d);

        uint8_t h,m,s;
        get_AABBCC(RMC.UTC_TIME, &h, &m, &s);
        printf("time: %02d:%02d:%02d\n", h,m,s);

        double lat = atof(RMC.Lat);
        printf("lat: %.8f, dir: %c\n", lat, RMC.Lat_dir);

        double lon = atof(RMC.Lon);
        printf("lon: %.8f, dir: %c\n", lon, RMC.Lon_dir);

        double lat_num = get_lat_lon(RMC.Lat, 1);
        double lon_num = get_lat_lon(RMC.Lon, 0);
        printf("%.8f\n", lat_num);
        printf("%.8f\n", lon_num);

        printf("Check: %s\n", RMC.Check);

//        // 16进制传输
//        // 时间
//        int Y = y + 2000;
//        storeToByteArray(Y, dataTime.year, 2, 1);
////        dataTime.year[0] = (Y >> 8) & 0xFF;
////        dataTime.year[1] = Y & 0xFF;
//        dataTime.month = M;
//        dataTime.day = d;
//
//        // 天秒最大 86400
//        int day_sec = h * 60 * 60 + m * 60 + s;
//        storeToByteArray(day_sec, dataTime.day_sec, 3, 1);
////        dataTime.day_sec[0] = (day_sec >> 16) & 0xFF;
////        dataTime.day_sec[1] = (day_sec >> 8) & 0xFF;
////        dataTime.day_sec[2] = day_sec & 0xFF;
//
//        // 毫秒
//        uint16_t milli_sec = (RMC.UTC_TIME[7] - 48) * 10 + (RMC.UTC_TIME[8] - 48);
//        storeToByteArray(milli_sec, dataTime.milli_sec, 2, 1);
////        dataTime.milli_sec[0] = (milli_sec >> 8) & 0xFF;
////        dataTime.milli_sec[1] = milli_sec & 0xFF;


        printf("\n");
    } else {
        printf("RMC: NO\n");
    }
    return index;
}

int GGA_test(uint8_t *test) {
    struct GGA_Body_Raw GGA = {{0}, {0}, 0, {0}, 0, {0}, {0}, {0}, {0}, {0}, {0}};

    int index = GGA_data_get(test, &GGA, ALL);

    if (index != 0) {
        printf("GGA: YES OK index is %d\r\n", index);

        uint8_t h,m,s;
        get_AABBCC(GGA.UTC_TIME, &h, &m, &s);
        printf("time: %02d:%02d:%02d\n", h,m,s);

        double lat = atof(GGA.Lat);
        printf("lat: %.8f, dir: %c\n", lat, GGA.Lat_dir);

        double lon = atof(GGA.Lon);
        printf("lon: %.8f, dir: %c\n", lon, GGA.Lon_dir);

        printf("Pos_qua: %s\n", GGA.Pos_qua);

        printf("satellites num: %s\n", GGA.Satellites_num);

        printf("HDOP: %s\n", GGA.HDOP);

        printf("Altitude: %s\n", GGA.Altitude);

        printf("Height: %s\n", GGA.Height);

        printf("Check: %s\n", GGA.Check);



//        int lat100 = (int)(lat * 100);
//        if(GGA.Lat_dir == 'S') lat100 = -lat100;
//        storeToByteArray(lat100, dataLtd.lat, 4, 1);
//
//        int lon100 = (int)(lon * 100);
//        if(GGA.Lon_dir == 'E') lon100 = -lon100;
//        storeToByteArray(lon100, dataLtd.lon, 4, 1);

        printf("\n");

    } else {
        printf("GGA: NO\r\n");
    }
    return index;
}

int GSV_test(uint8_t *test) {
    GSV_Body_Raw GSV = {0,0,{0},
                        {0}, {0}, {0}, {0},
                        {0}, {0}, {0}, {0},
                        {0}, {0}, {0}, {0},
                        {0}, {0}, {0}, {0},
                        0, {0}
    };

    int index = GSV_data_get(test, &GSV, ALL);

    if (index != 0) {
        printf("GSV: YES index is %d\n", index);

        printf("GSV_num: %c\n", GSV.GSV_num);

        printf("Index: %c\n", GSV.Index);

        int num = (GSV.Satellites_num[0] - 48) * 10 + (GSV.Satellites_num[1] - 48);
        printf("Satellites_num: %d\n", num);

        printf("\n");
    } else {
        printf("GSV: NO\n");
    }
    return index;
}


int main() {


    uint8_t *test = "$GPGGA,055258.000,3027.4932,N,11424.2381,E,1,06,2.9,101.9,M,-13.6,M,,0000*7E\n$GPGSA,A,3,28,06,57,58,30,02,,,,,,,4.1,2.9,2.9*35\n$GPRMC,055258.000,A,3027.4932,N,11424.2381,E,0.00,15.90,120620,,,A*5B\n\n$GPGGA,055259.000,3027.4932,N,11424.2381,E,1,06,2.9,101.9,M,-13.6,M,,0000*7F\n$GPGSA,A,3,28,06,57,58,30,02,,,,,,,4.1,2.9,2.9*35\n$GPRMC,055259.000,A,3027.4932,N,11424.2381,E,0.00,15.90,120620,,,A*5A\n\n$GPGGA,055300.000,3027.4932,N,11424.2381,E,1,06,2.9,101.9,M,-13.6,M,,0000*72\n$GPGSA,A,3,28,06,57,58,30,02,,,,,,,4.1,2.9,2.9*35\n$GPRMC,055300.000,A,3027.4932,N,11424.2381,E,0.00,15.90,120620,,,A*57\n\n$GPGGA,055301.000,3027.4932,N,11424.2381,E,1,06,2.9,102.0,M,-13.6,M,,0000*79\n$GPGSA,A,3,28,06,57,58,30,02,,,,,,,4.1,2.9,2.9*35\n$GPRMC,055301.000,A,3027.4932,N,11424.2381,E,0.00,15.90,120620,,,A*56\n\n$GPGGA,055302.000,3027.4932,N,11424.2381,E,1,06,2.9,102.0,M,-13.6,M,,0000*7A\n$GPGSA,A,3,28,06,57,58,30,02,,,,,,,4.1,2.9,2.9*35\n$GPGSV,3,1,12,28,82,174,34,06,41,252,36,57,34,129,31,58,32,165,39*77\n$GPGSV,3,2,12,30,13,199,39,02,10,244,36,01,14,046,21,03,39,063,26*78\n$GPGSV,3,3,12,10,50,145,,12,44,316,,19,43,060,,08,18,305,*77\n$GPRMC,055302.000,A,3027.4932,N,11424.2381,E,0.00,15.90,120620,,,A*55";

    int a1, a2, a3, t, l = 0;
    uint8_t *t2 = test;
    while (1) {
        a1 = RMC_test(t2);
        a2 = GGA_test(t2);
        a3 = GSV_test(t2);

        t = a1;
        if (a2 > t) t = a2;
        if (a3 > t) t = a3;

        if (t != 0) {
            l += t;
            t2 = t2 + t;
        }
        if (a1 == a2 && a2 == a3) {
            printf("\n--%d---%c\n", l, *(test + l + 2));
            break;
        }

        printf("\n--%d--\n",l);
        printf("\n\n\n");
    }



    return 0;
}
