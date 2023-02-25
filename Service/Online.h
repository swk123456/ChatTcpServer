#pragma once

#include<json/json.h>
#include<string>
#include<unordered_map>
#include<utility>
#include"../myTime/myTime.h"

using namespace std;

class Online {
private:
    struct user {
        int id;
        string username;
        string loginTime;
    };
    unordered_map<int, int> writeFdToReadFd;
    unordered_map<int, int> idToFd;
    unordered_map<int, user> userMap;

public:
    string getOnlineListStr();
    bool appendUser(int id, string username);
    bool removeUser(int id);
    bool appendUser(pair<int, string> &user);
    int getReadFd(int writeFd);
    vector<int> getAllReadFd();
    bool appendWriteFd(int id, int fd);
    bool appendReadFd(int id, int fd);
    string getUserJsonStr(int id);
    string getUserName(int id);
    bool isLogin(int id);
};