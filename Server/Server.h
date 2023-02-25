#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<json/json.h>
#include<errno.h>
#include"../ProtocolHead/HeadData.h"
#include"../Service/DataProcesser.h"
#include"../Service/UserService.h"
#include"../Service/Online.h"
#include"../config/server_config.h"

using namespace std;

class Server
{
private:
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
    int listen_fd;
    int epfd;
    struct epoll_event events[MAX_CONNECTIONS];//events回传要处理的事件

public:
    Server();
    ~Server();
    void Bind();
    void Listen();
    void Accept();
    void Run();
    void Recv(int fd);
};