#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<arpa/inet.h>
#include<cstdlib>
#include<unistd.h>
#include<sys/epoll.h>
#include<json/json.h>
#include<errno.h>
#include"ProtocolHead/HeadData.h"
#include"Service/DataProcesser.h"
#include"Service/UserService.h"
#include"Service/Online.h"
#include"config/server_config.h"
#include"Server/Server.h"

using namespace std;

int main()
{
    Server server;
    server.Bind();
    server.Listen();
    server.Run();
}
