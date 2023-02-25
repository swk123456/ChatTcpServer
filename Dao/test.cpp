#include"MySqlConnector.h"
#include<iostream>
using namespace std;

int main()
{
    MySqlConnector* c;
    c = MySqlConnector::getMySQLConnector();

    string username, password;
    cout << "username:";
    cin >> username;
    cout << "password:";
    cin >> password;
    cout << (c->insertUser(username, password)) << endl;
    //pair<int, string> user;
    //user = c->queryUser(username, password);
    //cout << user.first << "   " << user.second << endl;
    /*string id;
    cout << "id:";
    cin >> id;
    cout << c->queryUser(id) << endl;*/
    return 0;
}