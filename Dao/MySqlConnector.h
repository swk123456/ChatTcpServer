#pragma once

#include<iostream>
#include<string>
#include<mysql/mysql.h>
#include<unordered_map>
#include<json/json.h>
#include"../myTime/myTime.h"
#include"../config/mysql_config.h"
using namespace std;

class MySqlConnector
{
public:
    static MySqlConnector* getMySQLConnector();
    pair<int, string> queryUser(const string& id, const string& password);
    int queryUser(const string& id);
    int insertUser(const string& username, const string& password);
    ~MySqlConnector();

private:
    MYSQL *conn;
    void init();
};