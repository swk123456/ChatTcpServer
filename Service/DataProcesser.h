#pragma once

#include<iostream>
#include<string>
#include<unistd.h>
#include<vector>
#include<fstream>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include"../ProtocolHead/protocolMsg.h"
#include"../ProtocolHead/HeadData.h"
#include"../ProtocolHead/DataEncoder.h"
#include"../myTime/myTime.h"

#define IMAGE_PATH "./image/"
#define TCP_BUFSIZ 8192

using namespace std;

class DataProcesser
{
private:
    char buffer[TCP_BUFSIZ];
    int checkSocketConnected(int sock);

public:
    DataProcesser();
    
    string readTextContent(int fd, unsigned int dataLength);
    string readImageContent(int fd, unsigned int dataLength);

    void writeText(int fd, int id, string text, unsigned int protocolId = SEND);
    void writeImage(int fd, int id, const string& imagePath);
    void writeMsg(int fd, unsigned int id, string text, unsigned int protocolId = SEND);
    void writeTextToAllUser(const vector<int> &fds, int id, const string &text, unsigned int protocolId = SEND);
    void writeImageToAllUser(const vector<int> &fds, int id, const string &imagePath);

    int getFileLength(const string &fileName);
};