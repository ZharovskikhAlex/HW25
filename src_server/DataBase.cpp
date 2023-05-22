#include "DataBase.h"

DataBase::DataBase()
{
    mysql_init(&mysql);
    mysql_set_character_set(&mysql, "utf8");
    if (mysql_real_connect(&mysql, "localhost", "chatServer", "chatPassword", "ChatDB", 0, NULL, 0) == NULL)
    {
        std::cout << "Соединение с базой данных не было установлено!" << std::endl;
    }
    /*mysql_query(&mysql, "create database if not exists ChatDB");
    if (mysql_select_db(&mysql, "ChatDB") != 0)
    {
        std::cout << "База данных не была выбрана!" << std::endl;
    }*/
    mysql_query(&mysql, "create table if not exists users (u_id int auto_increment primary key, login varchar(255), password varchar(255))");
    mysql_query(&mysql, "create table if not exists chats (c_id int auto_increment primary key, user1 varchar(255), user2 varchar(255))");
    mysql_query(&mysql, "create table if not exists messages (m_id int auto_increment primary key, c_id int, author varchar(255), message varchar(1000))");
}

DataBase::~DataBase()
{
    mysql_close(&mysql);
}

bool DataBase::checkUser(std::string & login, std::string & password, char key)
{

    std::string query = "select login, password from users where ";
    query += "login = \'" + login + "\'";
    if (key == 'a')
    {
        query += "and password = \'" + password + "\'";
    }
    mysql_query(&mysql, &query[0]);
    res = mysql_store_result(&mysql);
    int a = res->row_count;
    mysql_free_result(res);
    if (a == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool DataBase::addUser(std::string & login, std::string & password)
{
    if (checkUser(login, password, 'r') == false)
    {
        std::string query = "insert into users (u_id, login, password) values(default, ";
        query += '\'' + login + "\', " + '\'' + password + "\')";
        mysql_query(&mysql, &query[0]);
        return true;
    }
    else
    {
        return false;
    } 
}

void DataBase::getUsersList(std::list<std::string> & userList)
{
    std::string query = "select login from users";
    mysql_query(&mysql, &query[0]);
    res = mysql_store_result(&mysql);
    MYSQL_ROW row;
    while (row = mysql_fetch_row(res))
    {
        for (size_t i = 0; i < mysql_num_fields(res); i++)
        {
            userList.push_back(row[i]);
        }
    }
    mysql_free_result(res);
}

void DataBase::getUserChats(std::list<std::string> & chatsList, std::string & activeUser)
{
    std::string query = "select * from chats where user1 = ";
    query += '\'' + activeUser + "\' or user2 = \'" + activeUser +'\'';
    mysql_query(&mysql, &query[0]);
    res = mysql_store_result(&mysql);
    MYSQL_ROW row;
    while (row = mysql_fetch_row(res))
    {
        for (size_t i = 0; i < mysql_num_fields(res); i++)
        {
            chatsList.push_back(row[i]);
        }
    }
    mysql_free_result(res);
}

void DataBase::addChat(std::string & user1, std::string & user2, std::string & c_id)
{
    std::string query = "insert into chats (c_id, user1, user2) values(default, ";
    query += '\'' + user1 + "\', " + '\'' + user2 + "\')";
    mysql_query(&mysql, &query[0]);
    query.clear();
    query = "select c_id from chats where user1 = ";
    query += '\'' + user1 + "\' and user2 = \'" + user2 + '\'';
    mysql_query(&mysql, &query[0]);
    res = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(res);
    c_id = row[0];
    mysql_free_result(res);
}

void DataBase::addMessage(std::string & c_id, std::string & author, std::string & message)
{
    std::string query = "insert into messages (m_id, c_id, author, message) values(default, ";
    query += '\'' + c_id + "\', " + '\'' + author + "\', " + '\'' + message + "\')";
    mysql_query(&mysql, &query[0]);
}

void DataBase::getChatMessages(std::string & c_id, std::list<std::string> & messagesList)
{
    std::string query = "select author, message from messages where c_id = ";
    query += '\'' + c_id + '\'';
    int a = mysql_query(&mysql, &query[0]);
    if (a != 0)
    {
        std::cout << query << std::endl;
    }
    res = mysql_store_result(&mysql);
    MYSQL_ROW row;
    while (row = mysql_fetch_row(res))
    {
        for (size_t i = 0; i < mysql_num_fields(res); i++)
        {
            messagesList.push_back(row[i]);
        }
    }
    mysql_free_result(res);
}