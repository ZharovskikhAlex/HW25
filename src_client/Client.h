#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <iterator>
#include "../src_shared/Socket.h"
#include "User.h"
#include "Chat.h"

#define PORT 7777

class Client
{
private:
    bool flag;
    int serverSocket;
    struct sockaddr_in serverAddress;
    User* user;
    Socket* sckt;
    void createUser();
    void getUsersList();
    void showUsersList();
    void addNewChat();
public:
    Client();
    ~Client() = default;
    void work();
    void startMenu();
    void userSession();
    void getUserChats();
    void showContacts();
    void writeMessage();
    void showMessChat(Chat &);
    void writeAll();
    bool inputMessage(std::string &);
};