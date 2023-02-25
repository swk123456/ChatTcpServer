#include"Online.h"

string Online::getOnlineListStr() {
    Json::Value onlineList;
    for(auto &item : userMap) {
        Json::Value userJson;
        userJson["id"] = item.second.id;
        userJson["username"] = item.second.username;
        userJson["loginTime"] = item.second.loginTime;
        onlineList.append(userJson);
    }
    return onlineList.toStyledString();
}

bool Online::appendUser(int id, string username) {
    user u = {id, move(username), myTime::getCurrentFormatTimeStr()};
    userMap[id] = u;
    return true;
}

bool Online::removeUser(int id) {
    userMap.erase(id);
    writeFdToReadFd.erase(idToFd[id]);
    idToFd.erase(id);
    return true;
}

bool Online::appendUser(pair<int, string> &user) {
    return appendUser(user.first, user.second);
}

int Online::getReadFd(int writeFd) {
    return writeFdToReadFd[writeFd];
}

vector<int> Online::getAllReadFd() {
    vector<int> v;
    for(auto &item : writeFdToReadFd) {
        v.push_back(item.second);
    }
    return v;
}

bool Online::appendWriteFd(int id, int fd) {
    idToFd[id] = fd;
    return true;
}

bool Online::appendReadFd(int id, int fd) {
    writeFdToReadFd[idToFd[id]] = fd;
    return true;
}

string Online::getUserJsonStr(int id) {
    Json::Value jsonUser;
    jsonUser["id"] = id;
    jsonUser["username"] = userMap[id].username;
    return jsonUser.toStyledString();
}

string Online::getUserName(int id) {
    return userMap[id].username;
}

bool Online::isLogin(int id) {
    return userMap.count(id) != 0;
}