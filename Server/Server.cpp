#include"Server.h"

Server::Server()
{
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    //server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    inet_pton(AF_INET, HOST, &server_addr.sin_addr);
    server_addr.sin_port = htons(PORT);

    //create socket to listen
    listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1) {
        cout << "Create listenSocket Fail!";
        exit(1);
    }

    int opt = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        cout << "socket:" << listen_fd << "   errno:" << errno <<endl;
        cout << "setsockopt fail" << endl;
        exit(-1);
    }//SOL_SOCKET套接字层次，SO_REUSERADDR允许重复使用本地地址
}

Server::~Server()
{
    close(epfd);
}

void Server::Bind()
{
    if((bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) == -1) {
        cout << "Server Bind Failed!" << endl;
        cout << "errno:" << errno << endl;
        exit(-1);
    }
    cout << "Bind Successfully!" << endl;
}

void Server::Listen() {
    if(listen(listen_fd, MAX_CONNECTIONS) == -1) {
        cout << "Server Listen Failed!" << endl;
        exit(-1);
    }
    cout << "Listen Successfully!" << endl;
}

void Server::Run()
{
    epfd = epoll_create(MAX_CONNECTIONS);//生成用于处理accept的epoll专用文件描述符

    struct epoll_event event;//ev注册事件
    event.data.fd = listen_fd;//设置与要处理的事件相关的文件描述符
    event.events = EPOLLIN;//对应的文件描述符可以读

    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &event);//在文件描述符epfd所引用的epoll实例上注册目标文件描述符fd，并将事件事件与内部文件链接到fd

    while(true)
    {
        int nums = epoll_wait(epfd, events, MAX_CONNECTIONS, -1);
        if(nums == -1) {
            cout << "epoll_wait Failed!" << endl;
            exit(1);
        }
        if(nums == 0) {
            continue;
        }
        for(int i = 0; i < nums; i++) {
            int fd = events[i].data.fd;
            if(fd == listen_fd && (events[i].events & EPOLLIN)) {
                Accept();
            } else if(events[i].events & EPOLLIN) {
                Recv(fd);
            }
        }
    }
}

void Server::Accept()
{
    cout << "accept request" << endl;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int new_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(new_fd == -1) {
        cout << "Server Accept Failed!" << endl;
        exit(1);
    }
    cout << "new connection was accepted!" << endl;

    struct epoll_event event;
    event.data.fd = new_fd;
    event.events = EPOLLIN;

    epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &event);

    struct timeval timeout = {1, 0};//时间 1秒 0微秒
    setsockopt(new_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval));//设置recv超时
}

void Server::Recv(int fd)
{
    cout << "receive request" << endl;
    struct epoll_event event;
    UserService us;
    Online online;
    HeadData hd(fd);
    if(!hd.readBuf()) {
        cout << "read fail" << endl;
        sleep(1);
        event.data.fd = fd;
        event.events = EPOLLIN;
        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
        close(fd);
        return;
    }
    unsigned int protocolId = hd.getProtocolId();
    unsigned int id = hd.getId();
    unsigned int dataType = hd.getDataType();
    unsigned int dataLength = hd.getDataLength();
    DataProcesser dp;
    switch(protocolId) {
        case LOGIN: {
            string loginMsg = dp.readTextContent(fd, dataLength);
            Json::Reader jsonReader;
            Json::Value msg;
            jsonReader.parse(loginMsg, msg);
            string id = msg["id"].asString();
            string password = msg["password"].asString();
            pair<int, string> user = us.checkLogin(id, password);
            Json::Value loginResult;
            if(user.first != 0) {
                if(online.isLogin(user.first)) {
                    loginResult["status"] = LOGIN_EXIST;
                } else {
                    online.appendUser(user);
                    online.appendWriteFd(user.first, fd);
                    loginResult["status"] = LOGIN_SUCCESS;
                    loginResult["username"] = user.second;
                }
                cout << "login id:" << user.first << endl;
            } else {
                loginResult["status"] = LOGIN_FAIL;
                cout << "login failed id:" << user.first << endl;
            }
            string loginResultStr = loginResult.toStyledString();
            dp.writeMsg(fd, 0, loginResultStr);
        }
            break;
        case REGISTER: {
            string registerMsg = dp.readTextContent(fd, dataLength);
            Json::Reader jsonReader;
            Json::Value registerResult;
            Json::Value msg;
            jsonReader.parse(registerMsg, msg);
            string username = msg["username"].asString();
            string password = msg["password"].asString();
            int id = us.registerUser(username, password);
            if(id == 0) {
                registerResult["status"] = REGISTER_FAIL;
            } else {
                registerResult["status"] = REGISTER_SUCCESS;
            }
            registerResult["id"] = id;
            dp.writeMsg(fd, 0, registerResult.toStyledString(), REGISTER);
            cout << "register id:" << id << endl;
        }
            break;
        case SEND: {
            string baseMsg = online.getUserName(id) + "(" + to_string(id) + ")说:";
            if(dataType == TEXT) {
                dp.writeTextToAllUser(online.getAllReadFd(), id, baseMsg);
                string content = dp.readTextContent(fd, dataLength);
                dp.writeTextToAllUser(online.getAllReadFd(), id, content);
            } else if(dataType == IMAGE) {
                string imagePath = dp.readImageContent(fd, dataLength);
                if(dp.getFileLength(imagePath) == dataLength) {
                    dp.writeTextToAllUser(online.getAllReadFd(), id, baseMsg);
                    dp.writeImageToAllUser(online.getAllReadFd(), id, imagePath);
                } else {
                    event.data.fd = fd;
                    event.events = EPOLLIN;
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
                    close(fd);
                    close(online.getReadFd(fd));
                    string logoutMsg = online.getUserName(id) + "(" + to_string(id) + ")离开了聊天室!";
                    online.removeUser(id);
                    vector<int> fds = online.getAllReadFd();
                    if(!fds.empty()) {
                        dp.writeTextToAllUser(fds, id, logoutMsg, NOTICE);
                        dp.writeTextToAllUser(fds, 0, online.getOnlineListStr(), ONLINELIST);
                    }
                }
            }
        }
            break;
        case READ: {
            event.data.fd = fd;
            event.events = EPOLLIN;
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
            online.appendReadFd(id, fd);
            string loginMsg = online.getUserName(id) + "(" + to_string(id) + ")走进了聊天室!";
            dp.writeTextToAllUser(online.getAllReadFd(), id, loginMsg, NOTICE);
            dp.writeTextToAllUser(online.getAllReadFd(), id, online.getOnlineListStr(), ONLINELIST);
        }
            break;
        case LOGOUT: {
            sleep(1);
            event.data.fd = fd;
            event.events = EPOLLIN;
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
            close(fd);
            close(online.getReadFd(fd));
            string logoutMsg = online.getUserName(id) + "(" + to_string(id) + ")离开了聊天室!";
            online.removeUser(id);
            vector<int> fds = online.getAllReadFd();
            cout << "online total member:" << fds.size() << endl;
            if(!fds.empty()) {
                dp.writeTextToAllUser(fds, id, logoutMsg, NOTICE);
                dp.writeTextToAllUser(fds, 0, online.getOnlineListStr(), ONLINELIST);
            }
        }
            break;
        case CLOSE: {
            sleep(1);
            event.data.fd = fd;
            event.events = EPOLLIN;
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
            close(fd);
        }
            break;
    }
}