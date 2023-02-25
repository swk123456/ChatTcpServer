#include "UserService.h"

bool UserService::checkID(string& id) {
    for(char &c : id) {
        if(!(c >= '0' && c <= '9')) {
            cout << "账号只能是数字" << endl;
            return false;
        }
    }
    return true;
}

bool UserService::checkUsername(string& username) {
    if(username.length() < 1 || username.length() > 20) {
        cout << "账号长度有误" << endl;
        return false;
    }
    return true;
}

bool UserService::checkPassword(string& password) {
    if(password.length() < 6 || password.length() > 20) {
        cout << "密码长度有误" << endl;
        return false;
    }
    return true;
}

UserService::UserService() {
    connector = MySqlConnector::getMySQLConnector();
}

pair<int, string> UserService::checkLogin(string id, string password) {
    pair<int, string> user;
    if(checkID(id) && checkPassword(password)) {
        user = connector->queryUser(id, password);
    }
    return user;
}

bool UserService::isRegistered(string id) {
    if(checkID(id)) {
        return false;
    }
    return connector->queryUser(id);
}

int UserService::registerUser(string username, string password) {
    if(checkUsername(username) && checkPassword(password)) {
        return connector->insertUser(username, password);
    }
    return 0;
}