#pragma once
#include <string>
#include <list>
#include <iostream>
#include "/usr/include/mysql/mysql.h"

class DataBase
{
private:
    MYSQL mysql;
    MYSQL_RES* res;
    int u_id, c_id, m_id;
public:
    DataBase();
    ~DataBase();
    bool checkUser(std::string &, std::string &, char key);
    bool addUser(std::string &, std::string &);
    void getUsersList(std::list<std::string> &);
    void getUserChats(std::list<std::string> &, std::string &);
    void addChat(std::string &, std::string &, std::string &);
    void addMessage(std::string &, std::string &, std::string &);
    void getChatMessages(std::string &, std::list<std::string> &);
};