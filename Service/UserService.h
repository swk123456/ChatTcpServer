#pragma once
#include"../Dao/MySqlConnector.h"

using namespace std;

class UserService {
private:
    MySqlConnector *connector;

    bool checkID(string &id);
    bool checkUsername(string &username);
    bool checkPassword(string &password);

public:
    UserService();

    pair<int, string> checkLogin(string id, string password);
    bool isRegistered(string id);
    int registerUser(string username, string password);
};