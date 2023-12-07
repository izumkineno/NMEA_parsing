#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <thread>

#include "main.h"

static RMC_Body_Raw RMC = {{0}, 0, {0}, 0, {0}, 0, {0}, {0}, {0}, {0}, 0, 0, 0, 0};
static GGA_Body_Raw GGA = {{0}, {0}, 0, {0}, 0, {0}, {0}, {0}, {0}, {0}, {0}};
static GSV_Body_Raw GSV = {0,0,{0},{0}, {0}, {0}, {0},{0}, {0}, {0}, {0},{0}, {0}, {0}, {0},{0}, {0}, {0}, {0},0, {0}};
static CSM_Body_Raw CSM = {{0}, {0}};
static LPS_Body_Raw LPS = {{0}, {0}, {0}, {0}, {0}, {0}};
static ZDA_Body_Raw ZDA = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};


HANDLE windows_read_port(char* com, u8* buffer_cache, u16 size, u8* test, u16 &length) {
    // 打开串口
    HANDLE hSerial = CreateFile(_T(com), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port." << std::endl;
    }

    // 配置串口参数
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state." << std::endl;
        CloseHandle(hSerial);
    }

    dcbSerialParams.BaudRate = CBR_115200;  // 设置波特率为9600
    dcbSerialParams.ByteSize = 8;         // 设置数据位为8
    dcbSerialParams.StopBits = ONESTOPBIT; // 设置停止位为1
    dcbSerialParams.Parity   = NOPARITY;   // 无奇偶校验

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state." << std::endl;
        CloseHandle(hSerial);
    }

//    while (1) {
//        DWORD bytesRead;
//        bool f = ReadFile(hSerial, buffer_cache, size, &bytesRead, nullptr);
//        if (f) {
//            // 成功读取数据
//            if (bytesRead > 0) {
//                // 处理接收到的数据，例如打印到控制台
////                buffer[bytesRead] = '\0'; // 添加字符串结束符
////                std::cout << "buffer: " << bytesRead << std::endl << buffer_cache << std::endl;
//                memcpy(&test[length], buffer_cache, bytesRead);
//                length += bytesRead;
//                std::cout << std::endl
//                          << "********************************" << std::endl
//                          << "|| ===> port_length: " << length  << std::endl
//                          << "////////////////////////////////" << std::endl;
//            }
//        }
//        Sleep(300);
//    }
    return hSerial;
}


void last_index_change(u8* test, const u16 *str, u16 i, u16 index, u16 *last_index, const char *name) {
    if (index != 0) {
        // 处理最后匹配数据出现的位置
        *last_index = index > *last_index ? index : *last_index;
        u16 len = index > *str ? index - *str : 0;
        std::cout << name << i << "||" << *str << "::" << index << " - " << len << "||" << "last_index: " << *last_index << std::endl;
//        std::string_view t((char *) &test[*str], len);
//        std::cout << t << std::endl;
    }
}

// 115200 1秒的数据
const u16 size = 14400;
static u8 buffer_cache[size] = {0};
static u8 test[size * 2] = {0};
static u8 state[6] = {0};
static u16 start[6] = {0};


int main() {


    u16 last_index = 0;
    u16 length = 0;
    u16 index;

    char com[] = "COM4";

//    std::thread t1(windows_read_port, com, buffer_cache, size, test, std::ref(length));

    DWORD bytesRead;
    HANDLE hSerial = windows_read_port(com, buffer_cache, size, test, length);

    while (true) {

        bool f = ReadFile(hSerial, buffer_cache, size, &bytesRead, nullptr);
        if (f) {
            // 成功读取数据
            if (bytesRead > 0) {
                // 处理接收到的数据，例如打印到控制台
//                buffer[bytesRead] = '\0'; // 添加字符串结束符
//                std::cout << "buffer: " << bytesRead << std::endl << buffer_cache << std::endl;
                memcpy(&test[length], buffer_cache, bytesRead);
                length += bytesRead;
                std::cout << std::endl
                          << "********************************" << std::endl
                          << "|| ===> port_length: " << length  << std::endl
                          << "////////////////////////////////" << std::endl;
            }
        }

        std::cout << std::endl
        << "====================================================================================" << std::endl
        << "buffer: " << length << std::endl
        << test << std::endl
        << "------------------------------------------------------------------------------------" << std::endl;


        for (u16 i = 0; i < length; ++i) {
            u8 protocol = 0;
            index = GGA_data_get(&GGA, test, i, ALL, &state[protocol], &start[protocol]);
            last_index_change(test, &start[protocol], i, index, &last_index, "GGA: ");
            ++protocol;
            index = RMC_data_get(&RMC, test, i, ALL, &state[protocol], &start[protocol]);
            last_index_change(test, &start[protocol], i, index, &last_index, "RMC: ");
            ++protocol;
            index = GSV_data_get(&GSV, test, i, ALL, &state[protocol], &start[protocol]);
            last_index_change(test, &start[protocol], i, index, &last_index, "GSV: ");
            ++protocol;
            index = CSM_data_get(&CSM, test, i, ALL, &state[protocol], &start[protocol]);
            last_index_change(test, &start[protocol], i, index, &last_index, "CSM: ");
            ++protocol;
            index = LPS_data_get(&LPS, test, i, ALL, &state[protocol], &start[protocol]);
            last_index_change(test, &start[protocol], i, index, &last_index, "LPS: ");
            ++protocol;
            index = ZDA_data_get(&ZDA, test, i, ALL, &state[protocol], &start[protocol]);
            last_index_change(test, &start[protocol], i, index, &last_index, "ZDA: ");
        }

        if (last_index != 0) {
            memmove(test, test + last_index, last_index);
            length -= length > last_index + 1 ? last_index + 1 : length;
            last_index = 0;
        }

        Sleep(800);
    }

}
