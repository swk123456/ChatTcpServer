#pragma once

#include"protocolMsg.h"
#include<unistd.h>
#include<arpa/inet.h>
#include<iostream>
using namespace std;

class HeadData {
    //协议号(1B)-账号(2B)-数据类型(1B)-数据长度(4B)-数据
private:
    int fd;
    char buffer[BASE_BUFFER_SIZE];
    const char *bp;
    unsigned int protocolId;
    unsigned int id;
    unsigned int dataType;
    unsigned int dataLength;

    void baseParse();
    unsigned int parseInt(int len);

public:
    HeadData(int fd);
    HeadData();
    bool parse(const char *buffer);
    bool readBuf();
    unsigned int getProtocolId();
    unsigned int getId();
    unsigned int getDataType();
    unsigned int getDataLength();
};