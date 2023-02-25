#include"MySqlConnector.h"

void MySqlConnector::init()
{
    conn = mysql_init(nullptr);
    conn = mysql_real_connect(conn, SQL_HOST, USERNAME, PASSWORD, DATABASE, SQL_PORT, NULL, 0);
    if(!conn) {
        cout << "mysql_real_connect fail" <<endl;
        exit(-1);
    }
    mysql_query(conn, "set names utf8");
}

MySqlConnector* MySqlConnector::getMySQLConnector()
{
    static MySqlConnector connector;
    connector.init();
    return &connector;
}

pair<int, string> MySqlConnector::queryUser(const string& id, const string& password)
{
    string queryUser = "select id, username from user where id = " + id + " and password = \"" + password + "\";";
    int res = mysql_query(conn, queryUser.data());
    pair<int, string> user{};
    if(!res) {
        MYSQL_RES *result = mysql_store_result(conn);
        if(result && mysql_num_rows(result)) {
            MYSQL_ROW row = mysql_fetch_row(result);
            user.first = atoi(row[0]);
            user.second = row[1];
        }
        mysql_free_result(result);
    }
    return user;
}

int MySqlConnector::queryUser(const string& id) {
    string querySql = "select id from user where id = " + id + ";";
    int res = mysql_query(conn, querySql.data());
    int id_ = 0;
    if(!res) {
        MYSQL_RES *result = mysql_store_result(conn);
        if(result && mysql_num_rows(result)) {
            MYSQL_ROW row = mysql_fetch_row(result);
            id_ = atoi(row[0]);
        }
        mysql_free_result(result);
    }
    return id_;
}

int MySqlConnector::insertUser(const string& username, const string& password)
{
    string createTime = myTime::getCurrentFormatTimeStr();
    string insertSql = "insert into user(username, password, createTime) values('"
     + username + "', '" + password + "', '" + createTime + "');";
    int res = mysql_query(conn, insertSql.data());
    if(res != 0) {
        return 0;
    }
    string querySql = "select id from user where username='" + username
     + "' and password='" + password + "' and createTime='" + createTime + "'";
    int i = mysql_query(conn, querySql.data());
    bool flag = false;
    int id = 0;
    if(!i) {
        MYSQL_RES *result = mysql_store_result(conn);
        if(result && mysql_num_rows(result)) {
            MYSQL_ROW row = mysql_fetch_row(result);
            id = atoi(row[0]);
        }
        mysql_free_result(result);
    }
    return id;
}

MySqlConnector::~MySqlConnector()
{
    mysql_close(conn);
}